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
build_arguments_list(struct argnode *args, struct scope *scope)
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
eval_args_for_match(struct argnode *args, struct scope *scope, int *argc_out)
	{
	struct value *argv;
	int n, i;
	struct argnode *a;
	/**/
	n = 0;
	for (a = args; a; a = argnode_next(a))
		n++;
	argv = calloc(n, sizeof *argv);
	if (!argv)
		{
		*argc_out = 0;
		return NULL;
		}
	i = 0;
	for (a = args; a; a = argnode_next(a), i++)
		{
		argv[i] = eval_expr(a->expr, scope);
		diagnose(DIAG_DEBUG, DBG_EXEC, 0, 0,
		         "arg etype=%d vtype=%d", a->expr->etype, argv[i].vtype);
		}
	*argc_out = n;
	return argv;
	}

static struct func *
find_matching_func(const char *name, struct value *argv, int argc,
                   struct scope *scope)
{
    struct func *f = sym_find_function(name, scope);
    while (f) {
        if (params_match(f, argv, argc) == MATCH_OK)
            return f;
        f = func_next_overload(f);
    }
    return NULL;
}
struct value
call_func(const char *name, struct argnode *args, struct scope *scope)
{
    int argc = 0;
    struct value *argv = eval_args_for_match(args, scope, &argc);
    struct func *f;
    struct value arguments;
    struct scope *local;
    struct value result;
    int i;

    f = find_matching_func(name, argv, argc, scope);
    if (!f) {
        report_match_failure(name, MATCH_TYPE_MISMATCH, NULL, argc);
        /* or keep the last match_result from the search if you want the precise reason */
        for (i = 0; i < argc; i++)
            Value_Free(argv[i]);
        free(argv);
        return Value_Empty();
    }

    /* build the arguments list from the already-evaluated values */
    arguments = Value_List(list_new());
    for (i = 0; i < argc; i++)
        list_append(argv[i], Value_AsList(arguments));

    local = scope_new(func_name(f), func_lexical_scope(f));
    sym_set_value("arguments", arguments, local);
    if (func_param_count(f) > 0)
        bind_fixed_params(f, arguments, local);

    if (func_builtin(f))
        result = func_builtin(f)(local);
    else
        result = exec_stmts(func_body(f), local);

    for (i = 0; i < argc; i++)
        Value_Free(argv[i]);
    free(argv);
    Value_Free(arguments);
    scope_free(local);
    return result;
}

/*
*return T_DECIMAL if either operand is decimal,
* else T_NUMBER if both are numbers,
* else T_UNTYPED
*/
enum vtype result_type(struct value a, struct value b)
	{
	int ret = T_UNTYPED;
	if ((Value_Type(a) == T_DECIMAL) || (Value_Type(a) == T_DECIMAL))
		ret = T_DECIMAL;
	else if ((Value_Type(b) == T_NUMBER) && (Value_Type(b) == T_NUMBER))
		ret = T_NUMBER;
	return ret;
	}
/* promote a number value to double; caller has already checked it is numeric */
static double as_double(struct value v)
	{
	return (Value_Type(v) == T_DECIMAL) ? Value_AsDecimal(v)
	       : (double)Value_AsNumber(v);
	}

struct value
eval_expr(struct expr *e, struct scope *scope)
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
				diagnose(DIAG_ERROR,DBG_EXEC,e->line,0, "undefined variable: %s\n", e->name);
				return z;
				}
			diagnose(DIAG_DEBUG, DBG_EXEC, 0, 0, "lookup %s type %d", e->name, Value_Type(s->val));
			return Value_Copy(s->val);
			}
		case EV_BINOP:
			struct value l = eval_expr(e->left, scope);
			struct value r = eval_expr(e->right, scope);
			struct value result = Value_Empty();
			enum vtype rt = result_type(l, r);
			if (rt != T_UNTYPED)
				{
				double ld = as_double(l);
				double rd = as_double(r);
				int is_comparison = 0;
				double res = 0.0;
				switch (e->op)
					{
					case PLUS: res = ld + rd; break;
					case MINUS: res = ld - rd; break;
					case TIMES: res = ld * rd; break;
					case OVER:
						if (rd == 0.0)
							diagnose(DIAG_WARNING, DBG_USER, e->line, 0, "division by zero");
						res = (rd != 0.0) ? ld / rd : 0.0;
						break;
					case GREATER_THAN: res = ld >  rd; is_comparison = 1; break;
					case LESS_THAN: res = ld <  rd; is_comparison = 1; break;
					case EQUAL_TO: res = ld == rd; is_comparison = 1; break;
					case UNEQUAL_TO: res = ld != rd; is_comparison = 1; break;
					case AT_LEAST: res = ld >= rd; is_comparison = 1; break;
					case AT_MOST: res = ld <= rd; is_comparison = 1; break;
					default:
						diagnose(DIAG_ERROR, DBG_EXEC, e->line, 0, "unknown operator");
						break;
					}
				if (is_comparison)
					result = Value_Bool(res != 0.0);
				else
					result = (rt == T_DECIMAL) ? Value_Decimal(res)
					         : Value_Number((long)res);
				}
			else
				{
				diagnose(DIAG_ERROR, DBG_EXEC, e->line, 0,
				         "non-numeric operands for operator");
				}
			Value_Free(l);
			Value_Free(r);
			return result;
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
exec_stmts(struct stmt *s, struct scope *scope)
	{
	struct value result = Value_Empty();
	while (s)
		{
		diagnose(DIAG_DEBUG,DBG_EXEC,0,0,"exec stype=%d\n", s->stype);
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
				diagnose(DIAG_DEBUG, DBG_EXEC, 0, 0,
				         "ST_CALL received empty=%d\n", Value_IsEmpty(r));
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
