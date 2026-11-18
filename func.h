/* func.h — additions for the new call path */
#ifndef FUNC_H
#define FUNC_H

#include "value.h"
#include "ast.h"
#include "symtab.h"
#include "params.h"

struct expr;

struct func
	{
	char *name;
	struct param *params;
	int param_count;
	struct stmt *body;
	struct scope *lexical_scope;
	struct func *next_overload;
	struct value (*builtin)(struct scope *);
	int is_rest;
	};


struct func *func_new(void);
struct func *func_create(const char *name, struct scope *lexical);
void         func_set_body(struct func *f, struct stmt *body);
void         func_set_builtin(struct func *f,
			      struct value (*fn)(struct scope *));
struct func *func_next_overload(const struct func *f);
int func_is_rest(const struct func *f);
void func_set_is_rest(struct func *f, int is_rest);


void func_add_param(struct func *f, const char *name, struct value init);
int  func_param_count(const struct func *f);
const char *func_param_name(const struct func *f, int i);
int  func_param_vtype(const struct func *f, int i);
int  func_param_is_rest(const struct func *f, int i);

const char *func_name(const struct func *f);
struct scope *func_lexical_scope(const struct func *f);
struct stmt *func_body(const struct func *f);
struct value (*func_builtin(const struct func *f))(struct scope *);

void         func_free(struct func *f);
struct func *func_define(char *name, struct param *params, struct stmt *body,
			 struct scope *scope);

struct func *
func_create_builtin(const char *name, struct scope *lexical,
                    struct value (*fn)(struct scope *), int is_rest);

#endif /* FUNC_H */
