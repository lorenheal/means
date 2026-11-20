#ifndef SYMTAB_H
#define SYMTAB_H


#include "means.h"
#include "value.h"   /* enum vtype */
#include "params.h"   /* enum vtype */
#include "func.h"      /* struct func, struct expr, struct stmt */

struct sym
	{
	char        *name;
	enum vtype   vtype;
	struct value val;
	struct func *func;        /* non-NULL when this symbol is a verb */
	struct sym  *next;
	};

struct scope
	{
	char         *name;       /* debugging only */
	struct sym   *syms;
	struct scope *outer;
	};

struct value sym_value(struct sym *s);
extern struct scope *global_scope;

struct scope *scope_new(const char *name, struct scope *outer);
void          scope_free(struct scope *s);

struct sym   *sym_find_in_scope(const char *name, struct scope *scope);
struct sym   *sym_find(const char *name, struct scope *scope);
struct sym   *sym_lookup_or_create(const char *name, struct scope *scope);
void          sym_set_value(const char *name, struct value v, struct scope *scope);
void          sym_declare(const char *name, enum vtype type, struct scope *scope);

#define sym_get_value(N,S) (sym_value(sym_find_in_scope((N),(S))))

void          sym_define_function(const char *name, struct func *f, struct scope *scope);
struct func  *sym_find_function(const char *name, struct scope *scope);


#endif /* SYMTAB_H */
