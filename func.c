/* func.c */
#include <stdlib.h>
#include <string.h>
#include "help.h"
#include "func.h"
#include "params.h"

struct expr; 

struct func *
func_new(void)
	{
	struct func *ret = calloc(1, sizeof *ret);
	if (!ret)
		{
		diagnose(DIAG_ERROR, DBG_AST, 0, 0, "out of memory in func_new");
		exit(1);
		}
	return ret;
	}

struct func *
func_create(const char *name, struct scope *lexical)
	{
	struct func *f = func_new();
	f->name = strdup(name);
	f->lexical_scope = lexical;
	return f;
	}

struct func *
func_create_builtin(const char *name, struct scope *lexical,
                    struct value (*fn)(struct scope *), int is_rest)
	{
	struct func *f = func_create(name, lexical);
	func_set_builtin(f, fn);
	func_set_is_rest(f, is_rest);
	return f;
	}

void
func_set_body(struct func *f, struct stmt *body)
	{
	f->body = body;
	}

void
func_set_builtin(struct func *f, struct value (*fn)(struct scope *))
	{
	f->builtin = fn;
	}

int
func_param_is_rest(const struct func *f, int i)
	{
	if (!f || i < 0 || i >= f->param_count)
		return 0; /* WARN: out-of-range index */
	return f->params[i].is_rest;
	}

void
func_set_is_rest(struct func *f, int is_rest)
	{
	if (f)
		f->is_rest = is_rest;
	}

struct func *
func_next_overload(const struct func *f)
	{
	return f ? f->next_overload : NULL;
	}

struct func *
func_define(char *name, struct param *params, struct stmt *body,
            struct scope *scope)
	{
	struct func *f = func_new();
	struct param *p;
	int n=0;
	f->name = name;
	f->params = params;
	f->body = body;
	f->lexical_scope = scope;
		for (p = params; p; p = p->next)
		n++;
	/* FIXME above should use accessors */
	f->param_count = n;
	diagnose(DIAG_DEBUG, DBG_GRAMMAR, 0, 0, "defined %s with %d params", name, n);
	sym_define_function(name, f, scope);
	return f;
	}

void func_add_param(struct func *f, const char *name, struct value init)
	{
	if (!f || !name)
		return; /* ERROR: early return — diagnose null */
	f->params = param_append(f->params, name, init);   /* stores the value */
	f->param_count++;
	}

int
func_param_count(const struct func *f)
	{
	return f ? f->param_count : 0;
	}

const char *
func_param_name(const struct func *f, int i)
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
func_param_vtype(const struct func *f, int i)
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
func_free(struct func *f)
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
func_is_rest(const struct func *f)
{
    return f && f->is_rest;   
}

const char *
func_name(const struct func *f)
	{
	return f ? f->name : NULL;
	}

struct scope *
func_lexical_scope(const struct func *f)
	{
	return f ? f->lexical_scope : NULL;
	}

struct stmt *
func_body(const struct func *f)
	{
	return f ? f->body : NULL;
	}

struct value
(*func_builtin(const struct func *f))(struct scope *)
	{
	return f ? f->builtin : NULL;
	}
