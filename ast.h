#ifndef AST_H
#define AST_H

#include "means.h"
#include "value.h"
#include "params.h"
#include "symtab.h"
#include "func.h"
#include "means.tab.h"

enum etype
	{
	EV_NUM,
	EV_WORD,
	EV_BOOL,
	EV_LIST,
	EV_VAR,
	EV_BINOP,
	EV_CALL
	};

enum stype
	{
	ST_ASSIGN,
	ST_RETURN,
	ST_IF,
	ST_WHILE,
	ST_CALL,
	ST_BLOCK
	};

struct expr
	{
	enum etype etype;
	int op;                     /* binop token or 0 */
	struct expr *left;
	struct expr *right;
	struct value val;           /* for literals; defined in symtab.h */
	char *name;                 /* for EV_VAR / EV_CALL */
	struct argnode *args;       /* for EV_CALL */
	int line;                   /* yylineno at expr_new */
	};

struct stmt
	{
	enum stype stype;
	struct stmt *next;
	char *name;                 /* assign / call */
	struct expr *expr;          /* assign / if / while / return */
	struct stmt *body;          /* if / while / block */
	struct stmt *else_body;     /* if */
	struct argnode *args;       /* call / return */
	int is_const;               /* assign */
	int line;                   /* yylineno at creation */
	};


struct stmt *stmt_append_list(struct stmt *list, struct stmt *s);
struct stmt *stmt_block(struct stmt *body);
struct stmt *stmt_assign(char *name, struct expr *expr, int is_const);
struct stmt *stmt_return(struct argnode *args);
struct stmt *stmt_if(struct expr *cond, struct stmt *thenb, struct stmt *elseb);
struct stmt *stmt_while(struct expr *cond, struct stmt *body);
struct stmt *stmt_call(char *name, struct argnode *args);

struct argnode *pack_if_multi(struct argnode *args);

struct expr * expr_new(enum etype et);

struct expr *ev_binop(int op, struct expr *left, struct expr *right);
struct expr *ev_number(int ival);
struct expr *ev_decimal(float fval);
struct expr *ev_word(char *s);
struct expr *ev_bool(int b);
struct expr *ev_list(struct argnode *nodes);
struct expr *ev_var(char *name);
struct expr *ev_call(char *name, struct argnode *args);


/* for tree walking */
struct expr *expr_left(const struct expr *e);
struct expr *expr_right(const struct expr *e);
struct stmt *stmt_body(const struct stmt *s);
struct stmt *stmt_else(const struct stmt *s);
struct stmt *stmt_next(const struct stmt *s);
void free_expr(struct expr *e);
void free_stmts(struct stmt *s);

void print_expr(struct expr *e);
void print_stmts(struct stmt *s);


#endif /* AST_H */
