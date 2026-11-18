#include "func.h"
#include "params.h"
#include "symtab.h"
#include "ast.h"

#ifndef EXEC_H
#define EXEC_H



struct value exec_func(struct func *f, struct value *argv, int argc,
                       struct scope *caller_scope);
struct value eval_expr(struct expr *e, struct scope *scope);
struct value exec_stmts(struct stmt *s, struct scope *scope);

#endif /* EXEC_H */
