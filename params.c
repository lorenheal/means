#include "params.h"
#include "meanslist.h"
#include "value.h"
#include "symtab.h"
#include "func.h"
#include "help.h"

#include <stdlib.h>
#include <string.h>

struct param *param_new(const char *name, struct value init)
{
    struct param *p = calloc(1, sizeof *p);
    if (!p)
		{
		diagnose(DIAG_ERROR, DBG_PARAMS, 0,0,"memory alloc fail in param_new");
        return NULL;
		}
    p->name = strdup(name);
	p->vtype = init.vtype;
    p->init = Value_Copy(init);   /* store the compile-time value */
    p->next = NULL;
    return p;
}

struct param *param_append(struct param *list, const char *name, struct value init)
{
    struct param *p = param_new(name, init);
    if (!list)
        return p;
    struct param *tail = list;
    while (tail->next)
        tail = tail->next;
    tail->next = p;
    return list;
}

struct param *param_from_decl(const char *name, struct expr *e, struct scope *scope)
	{
	int is_rest = (e->etype == EV_CALL && e->name &&
	               !e->args && strcmp(e->name, "rest") == 0);
	struct value v = is_rest
	                 ? Value_List(list_new())
	                 : eval_expr(e, scope);
    struct param *p = param_new(name, v);
    if (is_rest)
        p->is_rest = 1;
    return p;
	}

struct param *param_append_from_decl(struct param *list, const char *name,
                                     struct expr *e, struct scope *scope)
	{
	struct param *p = param_from_decl(name, e, scope);
	return param_append(list, p);
	}

void
param_free(struct param *list)
	{
	while (list)
		{
		struct param *n = list->next;
		free(list->name);
		free(list);
		list = n;
		}
	}

struct argnode *
argnode_next(struct argnode *a)
	{
	return((a)?(a->next):(NULL));
	}

struct argnode *
argnode_new(struct expr *e)
	{
	struct argnode *a = calloc(1, sizeof *a);
	if (!a)
		{
		diagnose(DIAG_ERROR, DBG_PARAMS, 0, 0, "out of memory");
		exit(1);
		}
	a->expr = e;
	return a;
	}

struct argnode *
arglist_new(struct expr *e)
	{
	return argnode_new(e);
	}

struct argnode *
arglist_append(struct expr *e, struct argnode *list)
	{
	struct argnode *n = argnode_new(e);
	if (!list)
		return n;
	struct argnode *t = list;
	while (t->next)
		t = t->next;
	t->next = n;
	return list;
	}

void
arglist_free(struct argnode *list)
	{
	while (list)
		{
		struct argnode *n = list->next;
		free(list);
		list = n;
		}
	}

match_result
params_match(const struct func *f, struct value *argv, int argc)
{
    const struct param *p;
    int i, fixed;

    if (!f)
        return MATCH_NULL_FUNC;

    fixed = func_param_count(f);
    if (argc < fixed)
        return MATCH_TOO_FEW;
    if (argc > fixed && !func_is_rest(f))
        return MATCH_TOO_MANY;

    for (p = f->params, i = 0; p && i < fixed; p = p->next, i++) {
        enum vtype want = p->vtype;
        struct value got;

        if (want == T_UNTYPED || want == T_ANY)
            continue;

        got = argv[i];
        if (want != T_LIST
            && Value_IsList(got)
            && list_length(Value_AsList(got)) == 1)
            got = list_first(Value_AsList(got));

        if (want != Value_Type(got)) {
            diagnose(DIAG_DEBUG, DBG_PARAMS, 0, 0,
                     "type mismatch want %d got %d",
                     want, Value_Type(got));
            return MATCH_TYPE_MISMATCH;
        }
    }
    return MATCH_OK;
}

void
report_match_failure(const char *name, match_result mr,
                     const struct func *f, int argc)
{
	int need = f ? func_param_count(f) : 0;

    switch (mr) 
	{
    case MATCH_TOO_FEW:
        diagnose(DIAG_ERROR, DBG_PARAMS, 0, 0,
                 "%s: too few arguments (got %d, need %d)",
                 name, argc, need);
        break;
    case MATCH_TOO_MANY:
        diagnose(DIAG_ERROR, DBG_PARAMS, 0, 0,
                 "%s: too many arguments (got %d, need %d)",
                 name, argc, need);
        break;
    case MATCH_TYPE_MISMATCH:
        diagnose(DIAG_ERROR, DBG_PARAMS, 0, 0,
                 "%s: argument type mismatch", name);
        break;
    case MATCH_NULL_FUNC:
    default:
        diagnose(DIAG_ERROR, DBG_PARAMS, 0, 0,
                 "undefined function: %s", name);
        break;
    }
}

void
bind_fixed_params(struct func *f, struct value arguments, struct scope *scope)
	{
	struct list *lst;
	struct param *p;
	int i;
	/**/
	if (!f || Value_IsEmpty(arguments) || !Value_IsList(arguments))
		/* ERROR: early return — diagnose “bad arguments” */
		return;
	lst = Value_AsList(arguments);
	if (!lst)
		return;
	i = 0;
	for (p = f->params; p && i < func_param_count(f); p = p->next, i++)
		{
		struct value v;
		enum vtype want;
		/**/
		if (list_isempty(lst))
			{
			diagnose(DIAG_ERROR, DBG_PARAMS, 0, 0,
				 "too few arguments for %s", func_name(f));
			return;
			}
		v = list_first(lst);
		want = p->vtype;
		/* unwrap one-element list for a scalar parameter */
		if (want != T_UNTYPED && want != T_ANY && want != T_LIST
		    && Value_IsList(v)
		    && list_length(Value_AsList(v)) == 1)
			{
			struct value elem = list_first(Value_AsList(v));
			Value_Free(v);
			v = elem;
			}
		if (want != T_UNTYPED && want != T_ANY && want != Value_Type(v))
			{
			diagnose(DIAG_ERROR, DBG_PARAMS, 0, 0,
				 "type mismatch for %s", p->name ? p->name : "?");
			Value_Free(v);
			return;
			}
		sym_set_value(p->name, v, scope);
		Value_Free(v);
		lst = list_rest(lst);   /* new header; original remains for rest */
		}
	}

void
params_free(struct param *params, int count)
	{
	int i;
	/**/
	if (!params)
		return;
	for (i = 0; i < count; i++)
		free(params[i].name);
	free(params);
	}
