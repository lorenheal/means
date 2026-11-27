#include "symtab.h"
#include "help.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct scope *global_scope = NULL;

struct scope *
scope_new(const char *name, struct scope *outer) /* def */
	{
	struct scope *s = calloc(1, sizeof *s);
	if (!s)
		{
		diagnose(DIAG_ERROR, DBG_SYMTAB,"scn", 0, 0, "out of memory");
		exit(1);
		}
	s->name = name ? strdup(name) : NULL;
	s->outer = outer;
	s->activation = NULL;
	return s;
	}

void dump_chain(struct scope *s, int depth)
	{
	FILE*out = stdout;
	struct sym *sym;
	for (; s; s = s->outer, depth++)
		{
		fprintf(out, "%*s%s (%p)", depth * 2, "",
		        s->name ? s->name : "?", (void *)s);
		if (s->activation)
			fprintf(out, "  act=%s (%p)",
			        s->activation->name ? s->activation->name : "?",
			        (void *)s->activation);
		for(sym = s->syms; sym; sym=sym->next)
			{
			char buf[24];
			fprintf(out, "%s", value_fmt(buf,24,sym_value(sym)));
			}
		fputc('\n', out);
		}
	}

void
scope_free(struct scope *s) /* def */
	{
	if (!s)
		return;
	/* WARN: symbols not freed  */
	free(s->name);
	free(s);
	}

static struct sym *sym_new(void) /* def */
	{
	struct sym *sym = calloc(1, sizeof *sym);
	if (!sym)
		{
		diagnose(DIAG_ERROR, DBG_SYMTAB,"syn", 0, 0, "out of memory");
		exit(1);
		}
	return(sym);
	}


void
sym_define_function(const char *name, struct func *f, struct scope *scope) /* def */
	{
	diagnose(DIAG_DEBUG, (DBG_SYMTAB & DBG_VERBOSE),"sdf1", 0, 0, "sym_define_function(%s, %p, %p)",
	         name ? name : "(null)", (void *)f, (void *)scope);
	if (!scope || !name || !f)  /* ERROR: null args — diagnose */
		return;
	diagnose(DIAG_DEBUG, (DBG_SYMTAB & DBG_VERBOSE),"sdf2", 0, 0, "defining %s in scope %p (%s)",
	         name, (void *)scope, scope->name ? scope->name : "?");
	struct sym *existing = sym_find_in_scope(name, scope);
	if (existing && existing->vtype == T_FUNC)
		{
		/* prepend to the overload list */
		f->next_overload = existing->func;
		existing->func = f;
		}
	else
		{
		struct sym *sym = sym_new();
		sym->name = strdup(name);
		sym->vtype = T_FUNC;
		sym->func = f;
		sym->next = scope->syms;
		scope->syms = sym;
		}
	}


struct sym *
sym_find_in_scope(const char *name, struct scope *scope) /* def */
	{
	struct sym *s = NULL;
	/**/
	if(scope && name)
		{
		for (s = scope->syms; s; s = s->next)
			if (strcmp(s->name, name) == 0)
				break;
		}
	return s;
	}

struct sym *
sym_find(const char *name, struct scope *scope) /* def */
	{
	struct sym *sym = NULL;
	/**/
	if(name)
		{
		for (; scope; scope = scope->outer)
			{
			if(scope->activation) 
				{
				sym = sym_find_in_scope(name, scope->activation);
				if (sym)
					break;
				}
			sym = sym_find_in_scope(name, scope);
			if (sym)
				break;
			}
		}
	return sym;
	}

struct func *
sym_find_function(const char *name, struct scope *scope) /* def */
	{
	struct sym *s = sym_find(name, scope);
	/**/
	diagnose(DIAG_DEBUG, (DBG_SYMTAB & DBG_VERBOSE),"sff", 0, 0,
	         "find %s in %p -> %p",
	         name, (void *)scope, (void *)s);
	if (s && s->vtype == T_FUNC)
		return s->func;
	return NULL;
	}

void
sym_set_value(const char *name, struct value v, struct scope *scope) /* def */
	{
	struct sym *s = sym_find_in_scope(name, scope);
	if(name && strcmp("x", name) == 0)
		{
		diagnose(DIAG_DEBUG, DBG_SYMTAB,"ssv", 0, 0,
		         "x: set %s type %d found sym %p", name, Value_Type(v), (void *) s);
		}
	if (!s)
		{
		s = sym_new();
		s->name = strdup(name);
		s->next = scope->syms;
		scope->syms = s;
		}
	Value_Free(s->val);
	s->val = Value_Copy(v);
	s->vtype = Value_Type(v);
	}

struct value sym_value(struct sym *s) /* def */
	{
	struct value ret = Value_Empty();
	if(s != NULL)
		ret = s->val;
	else
		diagnose(DIAG_DEBUG,DBG_SYMTAB,"sv",0,0,"NULL sym in sym_value");
	return ret;
	}

struct value sym_get_value(char *name, struct scope *scope) /* def */
	{
	struct value ret = Value_Empty();
	struct sym *s = NULL;
	if (name)
		{
		s = sym_find_in_scope(name, scope);
		}
	if(s)
		{
		ret = sym_value(s);
		}
	return (ret);
	}

/*
These may not be needed.
struct sym   *sym_lookup_or_create(const char *name, struct scope *scope);

void          sym_set_value(const char *name, struct value v, struct scope *scope);

void          sym_declare(const char *name, enum vtype type, struct scope *scope);

*/
