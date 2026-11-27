#include "symtab.h"
#include "ast.h"
#include "params.h"
#include "exec.h"
#include "meanslist.h"
#include "value.h"
#include "func.h"
#include "help.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static struct value
build_arguments_list(struct argnode *args, struct scope *scope) /* def */
	{
	struct list *lst;
	struct argnode *a;
	/**/
	lst = list_new();
	for (a = args; a; a = argnode_next(a))
		{
		struct value v = eval_expr(a->expr, scope);
		list_append(v, lst);
		Value_Free(v);          /* list_append copies */
		}
	return Value_List(lst);
	}

/* Temporary array only for overload matching; free the array, not the values. */
static struct value *
eval_args_for_match(struct argnode *args, struct scope *scope, int *argc_out) /* def */
	{
	struct value *ret = NULL;
	int n = 0;
	struct argnode *a;
	/**/
	if(!args)
		{
		n = 0;
		}
	else 
		{
		if(!argnode_next(args)) /* only one item */
			{
			/* single node: if its expression is not already a list, wrap it */
			struct expr *e = args->expr;
			if (e->etype != EV_LIST)
				{
				struct argnode *one = arglist_new(e);
				args = one;
				}
			}
			/* else it is already a list; keep it */
			/* multi-node arglist (the AND form) is already correct */
		for (a = args; a; a = argnode_next(a))
			n++;
		ret = calloc(n, sizeof *ret);
		if (!ret)
			{
			*argc_out = 0;
			/* FIXME diagnose memory error; FIXME more: argnode_new()  */
			}
		else
			{
			int i = 0;
			for (a = args; a; a = argnode_next(a), i++)
				{
				ret[i] = eval_expr(a->expr, scope);
				diagnose(DIAG_DEBUG, DBG_EXEC, "eafm", 0, 0, "arg[%d] etype=%d vtype=%d lval=%p",
						 i, a->expr->etype, ret[i].vtype,
						 (ret[i].vtype == T_LIST) ? (void *)ret[i].lval : NULL);
				}
			}
		}
	*argc_out = n;
	return ret;
	}

static struct func *
find_matching_func(const char *name, struct value *argv, int argc, /* def */
                   struct scope *scope, match_result *out_mr)
{
    struct func *f = sym_find_function(name, scope);
    match_result mr = MATCH_TYPE_MISMATCH;
    while (f) {
        mr = params_match(f, argv, argc);
        if (mr == MATCH_OK)
            break;
        f = func_next_overload(f);
    }
    if (out_mr)
        *out_mr = mr;
    return f;   /* NULL if none matched */
}

struct value
call_func(const char *name, struct argnode *args, struct scope *scope) /* def */
	{
	int argc = 0;
	struct value *argv = eval_args_for_match(args, scope, &argc);
	match_result mr = MATCH_NULL_FUNC;
	struct func *f = find_matching_func(name, argv, argc, scope, &mr);
	struct value result = Value_Empty();
	diagnose(DIAG_DEBUG, DBG_EXEC, "cf", 0, 0, "match %s %d mr: %d",
	         (f && f->name) ? f->name : "(null)", argc, mr);
	if (f)
		{
		struct scope *lex = func_lexical_scope(f);
		struct scope *parent = lex->outer;
		struct scope *local = scope_new(func_name(f), lex);
		if (parent)
			parent->activation = local;
		else
			lex->activation = local;
		if(DEBUG(DBG_EXEC)) dump_chain(local,0);
		struct value arguments = Value_Empty();
		if (argc > 0)
			{
			arguments = Value_List(list_new());
			for (int i = 0; i < argc; i++)
				list_append(argv[i], Value_AsList(arguments));
			}
		sym_set_value("arguments", arguments, local);
		if (func_param_count(f) > 0)
			bind_fixed_params(f, arguments, local);
		if (func_builtin(f))
			result = func_builtin(f)(local);
		else
			result = exec_stmts(func_body(f), local);
		if(parent)
			parent->activation = NULL;
		else
			lex->activation = NULL;
		Value_Free(arguments);
		scope_free(local);
		}
	else
		{
		report_match_failure(name, mr, NULL, argc);
		}
	for (int i = 0; i < argc; i++)
		Value_Free(argv[i]);
	free(argv);
	return result;
	}

/*
*return T_DECIMAL if either operand is decimal,
* else T_NUMBER if both are numbers,
* else T_UNTYPED
*/
enum vtype result_type(struct value a, struct value b) /* def */
	{
	int ret = T_UNTYPED;
	if ((Value_Type(a) == T_DECIMAL) || (Value_Type(a) == T_DECIMAL))
		ret = T_DECIMAL;
	else if ((Value_Type(b) == T_NUMBER) && (Value_Type(b) == T_NUMBER))
		ret = T_NUMBER;
	return ret;
	}
/* promote a number value to double; caller has already checked it is numeric */
static double as_double(struct value v) /* def */
	{
	return (Value_Type(v) == T_DECIMAL) ? Value_AsDecimal(v)
	       : (double)Value_AsNumber(v);
	}

struct value
eval_expr(struct expr *e, struct scope *scope) /* def */
	{
	struct value z = Value_Empty();
	if (!e)
		return z;
	switch (e->etype)
		{
		case EV_NUM:
		case EV_WORD:
		case EV_BOOL:
			return Value_Copy(e->val);
		case EV_VAR:
			{
			struct sym *s = sym_find(e->name, scope);
			if (!s)
				{
				diagnose(DIAG_ERROR,DBG_EXEC,"exv1",e->line,0, "undefined variable: %s\n", e->name);
				return z;
				}
			diagnose(DIAG_DEBUG, DBG_EXEC,"exv2", 0, 0, "lookup %s type %d", e->name, Value_Type(s->val));
			return Value_Copy(s->val);
			}
		case EV_BINOP:
			{
			struct value l = eval_expr(e->left, scope);
			struct value r = eval_expr(e->right, scope);
			struct value result = Value_Empty();

			switch (e->op)
				{
				/* logical — truth-value only */
				case LOGICAL_AND:
				case LOGICAL_OR:
				case EXCLUSIVE_OR:
					{
					int lb = Value_IsTrue(l);
					int rb = Value_IsTrue(r);
					int res = (e->op == LOGICAL_AND) ? (lb && rb)
					          : (e->op == LOGICAL_OR)  ? (lb || rb)
					          :                          (lb != rb);
					result = Value_Bool(res);
					break;
					}

				/* everything else is numeric / comparison */
				default:
					{
					enum vtype rt = result_type(l, r);
					if (rt == T_UNTYPED)
						{
						diagnose(DIAG_ERROR, DBG_EXEC,"exb", e->line, 0,
						         "non-numeric operands for operator");
						break;
						}
					double ld = as_double(l);
					double rd = as_double(r);
					int is_comparison = 0;
					double res = 0.0;

					switch (e->op)
						{
						case PLUS:
							res = ld + rd;
							break;
						case MINUS:
							res = ld - rd;
							break;
						case TIMES:
							res = ld * rd;
							break;
						case OVER:
							if (rd == 0.0)
								diagnose(DIAG_WARNING, DBG_USER,"exb", e->line, 0, "division by zero");
							res = (rd != 0.0) ? ld / rd : 0.0;
							break;
						case GREATER_THAN:
							res = ld >  rd;
							is_comparison = 1;
							break;
						case LESS_THAN:
							res = ld <  rd;
							is_comparison = 1;
							break;
						case EQUAL_TO:
							res = ld == rd;
							is_comparison = 1;
							break;
						case UNEQUAL_TO:
							res = ld != rd;
							is_comparison = 1;
							break;
						case AT_LEAST:
							res = ld >= rd;
							is_comparison = 1;
							break;
						case AT_MOST:
							res = ld <= rd;
							is_comparison = 1;
							break;
						default:
							diagnose(DIAG_ERROR, DBG_EXEC,"exb", e->line, 0, "unknown operator");
							break;
						}

					if (is_comparison)
						result = Value_Bool(res != 0.0);
					else
						result = (rt == T_DECIMAL) ? Value_Decimal(res)
						         : Value_Number((long)res);
					break;
					}
				}

			Value_Free(l);
			Value_Free(r);
			return result;
			}
		case EV_CALL:
			return call_func(e->name, e->args, scope);
		case EV_LIST:
			{
			struct list *lst = list_new();
			struct argnode *a;
			for (a = e->args; a; a = argnode_next(a))
				{
				struct value v = eval_expr(a->expr, scope);
				list_append(v, lst);
				Value_Free(v);
				}
			return Value_List(lst);
			}
		default:
			return z;

		}

	}

struct value
exec_stmts(struct stmt *s, struct scope *scope) /* def */
	{
	struct value result = Value_Empty();
	while (s)
		{
		diagnose(DIAG_DEBUG,DBG_EXEC,"es",0,0,"stype=%d\n", s->stype);
		switch (s->stype)
			{
			case ST_ASSIGN:
				{
				struct value v = eval_expr(s->expr, scope);
				sym_set_value(s->name, v, scope);
				Value_Free(v);
				break;
				}
			case ST_CALL:
				{
				struct value r = call_func(s->name, s->args, scope);
				diagnose(DIAG_DEBUG, DBG_EXEC,"esc", 0, 0,
				         "received empty=%d\n", Value_IsEmpty(r));
				Value_Free(r);
				break;
				}
			case ST_BLOCK:
				{
				struct value r = exec_stmts(s->body, scope);
				if (!Value_IsEmpty(r))
					return r;
				Value_Free(r);
				break;
				}
			case ST_RETURN:
				return build_arguments_list(s->args, scope);
			case ST_IF:
				{
				struct value cond = eval_expr(s->expr, scope);
				struct value r;
				if (Value_IsTrue(cond))
					r = exec_stmts(s->body, scope);
				else
					r = exec_stmts(s->else_body, scope);
				Value_Free(cond);
				if (!Value_IsEmpty(r))
					return r;
				Value_Free(r);
				break;
				}
			case ST_WHILE:
				{
				struct value cond = eval_expr(s->expr, scope);
				/* WARN: possible infinite loop if cond never becomes false */
				while (Value_IsTrue(cond))
					{
					struct value r = exec_stmts(s->body, scope);
					Value_Free(cond);
					if (!Value_IsEmpty(r))
						return r;
					Value_Free(r);
					cond = eval_expr(s->expr, scope);
					}
				Value_Free(cond);
				break;
				}
			default:
				break;
			}
		s = s->next;
		}
	return result;
	}
