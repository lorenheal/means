/* func.c */
#include <stdlib.h>
#include <string.h>
#include "help.h"
#include "func.h"
#include "params.h"

struct expr;

struct func *
func_new(void) /* def */
	{
	struct func *ret = calloc(1, sizeof *ret);
	if (!ret)
		{
		diagnose(DIAG_ERROR, DBG_AST, "fn", 0, 0, "out of memory in func_new");
		exit(1);
		}
	return ret;
	}

struct func *
func_create(const char *name, struct scope *lexical) /* def */
	{
	struct func *f = func_new();
	f->name = strdup(name);
	f->lexical_scope = lexical;
	return f;
	}

struct func *
func_create_builtin(const char *name, struct scope *lexical, /* def */
                    struct value (*fn)(struct scope *), int is_rest)
	{
	struct func *f = func_create(name, lexical);
	func_set_builtin(f, fn);
	func_set_is_rest(f, is_rest);
	return f;
	}

void show_func(struct func *f, const char *where, int line)
{
    if (!where) where = "";
    if (f)
        diagnose(DIAG_DEBUG, DBG_FUNC, where, line, 0,
                 "[%s p:%p n:%d body:%p lex:%p next:%p builtin:%p %s]\n",
                 f->name, (void *)f->params, f->param_count,
                 (void *)f->body, (void *)f->lexical_scope,
                 (void *)f->next_overload, (void *)f->builtin,
                 f->is_rest ? "rest" : "");
    else
        diagnose(DIAG_DEBUG, DBG_FUNC, where, 0, 0, "[:(nil)]");
}

void
func_set_body(struct func *f, struct stmt *body)/* def */
	{
	f->body = body;
	}

void
func_set_builtin(struct func *f, struct value (*fn)(struct scope *))/* def */
	{
	f->builtin = fn;
	}

/* obsolete - returns 0 */
int
func_param_is_rest(const struct func *f, int i) /* def */
	{
	if (!f || i < 0 || i >= f->param_count)
		return 0; /* WARN: out-of-range index */
	return f->params[i].is_rest;
	}

/* obsolete - no op  */
void
func_set_is_rest(struct func *f, int is_rest) /* def */
	{
	if (f)
		f->is_rest = is_rest; 
	}

struct func *
func_next_overload(const struct func *f) /* def */
	{
	return f ? f->next_overload : NULL;
	}

struct func *
func_define(char *name, struct param *params, struct stmt *body, struct scope *enclosing) /* def */
	{
	struct scope *local = scope_new(name, enclosing);
	struct func *f = func_new();
	struct param *p;
	int n = 0;
	int saw_rest = 0;
	/**/
	f->name          = name;
	f->params        = params;
	f->body          = body;
	f->lexical_scope = local;
	for (p = params; p; p = p->next)
		{
		if (saw_rest)
			{
			diagnose(DIAG_ERROR, DBG_PARAMS,"fd", 0, 0,
			         "%s: parameter after rest parameter", name);
			break;
			}
		sym_set_value(p->name, p->init, local);   /* default, else exemplar */
		if (p->is_rest)
			saw_rest = 1;
		n++;
		}
	func_set_is_rest(f, saw_rest);
	f->param_count = n;
	diagnose(DIAG_DEBUG, DBG_GRAMMAR,"fd", 0, 0, "defined %s with %d params in enclosing:%p", name, n, (void *)enclosing);
	sym_define_function(name, f, enclosing->outer?enclosing->outer:enclosing);
	return f;
	}

void func_add_param(struct func *f, const char *name, struct value init) /* def */
	{
	if (!f || !name)
		return; /* ERROR: early return — diagnose null */
	f->params = param_append(f->params, name, init);   /* stores the value */
	f->param_count++;
	}

int
func_param_count(const struct func *f) /* def */
	{
	int ret = 0;
	show_func( (struct func *) f, "fpc", 0);
	if(f && f->params)
		ret = f->param_count;
	return(ret);
	}

const char *
func_param_name(const struct func *f, int i) /* def */
	{
	const struct param *p;
	int n;
	/**/
	if (!f || i < 0)
		return NULL;
	for (p = f->params, n = 0; p && n < i; p = p->next, n++)
		;
	return p ? p->name : NULL;
	}

int
func_param_vtype(const struct func *f, int i) /* def */
	{
	const struct param *p;
	int n;
	/**/
	if (!f || i < 0)
		return T_UNTYPED;
	for (p = f->params, n = 0; p && n < i; p = p->next, n++)
		;
	return p ? p->vtype : T_UNTYPED;
	}

void
func_free(struct func *f) /* def */
	{
	if (!f)
		return;
	/**/
	free(f->name);
	param_free(f->params);
	/* body owned by the statement list */
	free(f);
	}

int
func_is_rest(const struct func *f) /* def */
	{
	return f && f->is_rest;
	}

const char *
func_name(const struct func *f) /* def */
	{
	return f ? f->name : NULL;
	}

struct scope *
func_lexical_scope(const struct func *f) /* def */
	{
	return f ? f->lexical_scope : NULL;
	}

struct stmt *
func_body(const struct func *f) /* def */
	{
	return f ? f->body : NULL;
	}

struct value
(*func_builtin(const struct func *f))(struct scope *) /* def */
	{
	return f ? f->builtin : NULL;
	}
