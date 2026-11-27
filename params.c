#include "params.h"
#include "meanslist.h"
#include "value.h"
#include "symtab.h"
#include "func.h"
#include "exec.h"
#include "help.h"

#include <stdlib.h>
#include <string.h>

struct param *param_new(const char *name, struct value init) /* def */
	{
	struct param *p = calloc(1, sizeof *p);
	if (!p)
		{
		diagnose(DIAG_FATAL, DBG_PARAMS,"pn", 0,0,"memory alloc fail in param_new");
		return NULL; /* never reached */
		}
	p->name = strdup(name);
	p->vtype = init.vtype;
	p->init = Value_Copy(init);   /* store the compile-time value */
	p->required = 0;
	p->next = NULL;
	return p;
	};

	/*
	struct param {
    char *name;
    enum vtype vtype;
    struct value init;          compile-time default 
    struct param *next;
    int required;    1 = IS LIKE, 0 = has default 
    int is_rest;
	*/

void show_param(struct param *p, const char *where, int line)
	{
	if (!where) where="where?";
	if(p)
		diagnose(DIAG_DEBUG,DBG_PARAMS,where,line,0,
			"[(%d) p:%s [def:%s] n:%p %s %s]", (int) p->vtype, p->name, 
			p->init, p->next, (p->required?"required":"default"), (p->is_rest?"rest":"not rest"));
	else
		diagnose(DIAG_DEBUG,DBG_PARAMS,where,line,0,"p:(nil)");
	}

struct param *param_append(struct param *list, const char *name, struct value init) /* def */
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

struct param *param_from_decl(const char *name, struct expr *exemplar,    /* def */
                              int has_default, struct scope *scope)
	{
    /* Discover type (and default value if has_default!=0) from the exemplar. */
    struct value v = eval_expr(exemplar, scope);
	struct param *p = param_new(name, v);
	if(Value_Type(v) ==T_LIST) /* FIXME */
		p->is_rest = 1;
	p->required = !has_default;
    Value_Free(v);
    return p;
	}

struct param *param_append_from_decl(struct param *list,  const char *name, /* def */
                                     struct expr *e, int init, struct scope *scope)
	{
	struct param *p = param_from_decl(name, e, init, scope);
	if (!p)
		return list;
	if (!list)
		return p;
	struct param *tail = list;
	while (tail->next)
		tail = tail->next;
	tail->next = p;
	return list;
	}

struct param *param_from_type(const char *name, enum vtype t, int required)
{
    struct value v = Value_EmptyType(t);
    struct param *p = param_new(name, v);   /* copies the empty value */
    p->required = required;
    return p;
}

struct param *param_link(struct param *list, struct param *p)
{
    if (!p) return list;
    if (!list) return p;
    struct param *tail = list;
    while (tail->next)
        tail = tail->next;
    tail->next = p;
    return list;
}

void
param_free(struct param *list) /* def */
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
argnode_next(struct argnode *a) /* def */
	{
	return((a)?(a->next):(NULL));
	}

struct argnode *
argnode_new(struct expr *e) /* def */
	{
	struct argnode *a = calloc(1, sizeof *a);
	if (!a)
		{
		diagnose(DIAG_ERROR, DBG_PARAMS, "an", 0, 0, "out of memory");
		exit(1);
		}
	a->expr = e;
	return a;
	}

struct argnode *
arglist_new(struct expr *e) /* def */
	{
	return argnode_new(e);
	}

struct argnode *
arglist_append(struct expr *e, struct argnode *list) /* def */
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
arglist_free(struct argnode *list) /* def */
	{
	while (list)
		{
		struct argnode *n = list->next;
		free(list);
		list = n;
		}
	}

match_result
params_match(const struct func *f, struct value *argv, int argc) /* def */
	{
	match_result result = MATCH_OK;
	const struct param *p;
	int i, fixed, required;
	diagnose(DIAG_DEBUG, DBG_PARAMS, "pm0", 0, 0,
         "enter %s argc=%d params=%p", f ? f->name : "(null)", argc,
         f ? (void *)f->params : NULL);
	if (!f)
		result = MATCH_NULL_FUNC;
	else
		{
		fixed = func_param_count(f);
		required = 0;
		for (p = f->params; p; p = p->next)
			{
			if(p->required)
				required++;
			}
		if (!(f->is_rest && fixed == 0))
			{
			if (argc < required)
				result = MATCH_TOO_FEW;
			else if (argc > fixed && !func_is_rest(f))
				result = MATCH_TOO_MANY;
			else
				{
				for (p = f->params, i = 0;
				        p && i < argc && result == MATCH_OK;
				        p = p->next, i++)
					{
					enum vtype want = p->vtype;
					struct value got = argv[i];
					if (p->is_rest)
						continue;   /* rest collects whatever remains */
					if (want != T_UNTYPED && want != T_ANY && want != T_LIST
					        && Value_IsList(got)
					        && list_length(Value_AsList(got)) == 1)
						got = list_first(Value_AsList(got));
					diagnose(DIAG_DEBUG, DBG_PARAMS, "pm1", 0, 0,
					         "want %d got %d", want, Value_Type(got));
					if (want != T_UNTYPED && want != T_ANY && want != Value_Type(got))
						result = MATCH_TYPE_MISMATCH;
					diagnose(DIAG_DEBUG, DBG_PARAMS, "pm2", 0, 0,
					         "%s want %d got %d", p->name, p->vtype, Value_Type(got));
					}
				}
			}
		}
	return result;
	}

void
report_match_failure(const char *name, match_result mr, /* def */
                     const struct func *f, int argc)
	{
	int need = -1 ; 
	if (!f)
		{
		if(!name) name = "(unnamed)";
        diagnose(DIAG_FATAL, DBG_PARAMS, "rmf0", 0, 0,
                 "match failure with null function (%s, argc %d)", name, argc);
		}
	else
		{
		switch (mr)
			{
			case MATCH_TOO_FEW:
				diagnose(DIAG_ERROR, DBG_PARAMS,"rmf1", 0, 0,
						 "%s: too few arguments (got %d, need %d)",
						 name, argc, need);
				break;
			case MATCH_TOO_MANY:
				diagnose(DIAG_ERROR, DBG_PARAMS,"rmf2", 0, 0,
						 "%s: too many arguments (got %d, need %d)",
						 name, argc, need);
				break;
			case MATCH_TYPE_MISMATCH:
				diagnose(DIAG_ERROR, DBG_PARAMS,"rmf3", 0, 0,
						 "%s: argument type mismatch", name);
				break;
			case MATCH_NULL_FUNC:
			default:
				diagnose(DIAG_ERROR, DBG_PARAMS,"rmf4", 0, 0,
						 "undefined function: %s", name);
				break;
			}
		}
	}

void
bind_fixed_params(struct func *f, struct value arguments, struct scope *scope)
	{
	struct list *lst = NULL;
	struct param *p;
	int i;
	if (!f)
		return;
	if (Value_IsList(arguments))
		lst = Value_AsList(arguments);
	i = 0;
	for (p = f->params; p && i < func_param_count(f); p = p->next, i++)
		{
		struct value v;
		enum vtype want = p->vtype;
		if (!lst || list_isempty(lst))
			{
			v = Value_Copy(p->init);
			}
		else
			{
			v = list_first(lst);
			if (want != T_UNTYPED && want != T_ANY && want != T_LIST
			        && Value_IsList(v)
			        && list_length(Value_AsList(v)) == 1)
				{
				struct value elem = list_first(Value_AsList(v));
				Value_Free(v);
				v = elem;
				}
			lst = list_rest(lst);
			}
		/* rest parameter receives the entire remaining list */
		if (p->is_rest)
			{
			struct list *rest_list = list_new();
			list_append(v, rest_list);
			while (lst && !list_isempty(lst))
				{
				list_append(list_first(lst), rest_list);
				lst = list_rest(lst);
				}
			Value_Free(v);
			v = Value_List(rest_list);
			}
		if (want != T_UNTYPED && want != T_ANY && want != Value_Type(v))
			{
			diagnose(DIAG_ERROR, DBG_PARAMS,"bfp", 0, 0,
			         "type mismatch for %s", p->name ? p->name : "?");
			Value_Free(v);
			return;
			}
		sym_set_value(p->name, v, scope);
		Value_Free(v);
		}
	}

void
params_free(struct param *params, int count) /* def */
	{
	int i;
	/**/
	if (!params)
		return;
	for (i = 0; i < count; i++)
		free(params[i].name);
	free(params);
	}
