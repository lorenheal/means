/* params.h — add the new helper */
#ifndef PARAMS_H
#define PARAMS_H

#include "ast.h"
#include "value.h"
#include "symtab.h"
#include "func.h"

#define MAX_PARAMS 1000

struct expr;

struct param {
    char *name;
    enum vtype vtype;
    struct value init;          /* compile-time default */
    struct param *next;
    int is_rest;
};

/* in params.h or near the function */
typedef enum {
    MATCH_OK = 0,
    MATCH_NULL_FUNC,
    MATCH_TOO_FEW,
    MATCH_TOO_MANY,
    MATCH_TYPE_MISMATCH
} match_result;

void report_match_failure(const char *name, match_result mr,
                     const struct func *f, int argc);

match_result params_match(const struct func *f, struct value *argv, int argc);


struct param *param_new(const char *name, struct value init);
struct param *param_append(struct param *list, const char *name, struct value init);
struct param *param_from_decl(const char *name, struct expr *e, struct scope *scope);
struct param *param_append_from_decl(struct param *list, const char *name,
                                     struct expr *e, struct scope *scope);

void          param_free(struct param *list);
void          bind_fixed_params(struct func *f, struct value arguments,
				struct scope *scope);

/* call-site argument list */
struct argnode
	{
	struct expr *expr;
	struct argnode *next;
	};

struct argnode *argnode_next(struct argnode *a);
struct argnode *arglist_new(struct expr *e);
struct argnode *arglist_append(struct expr *e, struct argnode *list);

void            arglist_free(struct argnode *list);


#endif
