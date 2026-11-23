#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "func.h"
#include "help.h"

extern int yylineno;

static struct stmt *
stmt_new(enum stype st)
	{
	struct stmt *s = calloc(1, sizeof *s);
	if (!s)
		{
		diagnose(DIAG_ERROR, DBG_AST, yylineno, 0, "out of memory");
		exit(1);
		}
	s->stype = st;
	s->line = yylineno;
	return s;
	}

static struct expr *
expr_new(enum etype et)
	{
	struct expr *e = calloc(1, sizeof *e);
	if (!e)
		{
		diagnose(DIAG_ERROR, DBG_AST, yylineno, 0, "out of memory");
		exit(1);
		}
	e->etype = et;
	e->line = yylineno;
	return e;
	}

struct stmt *
stmt_append_list(struct stmt *list, struct stmt *s)
	{
	if (!s)
		return list;
	if (!list)
		return s;
	struct stmt *t = list;
	while (t->next)
		t = t->next;
	t->next = s;
	return list;
	}

struct stmt *
stmt_block(struct stmt *body)
	{
	struct stmt *s = stmt_new(ST_BLOCK);
	s->body = body;
	return s;
	}

struct stmt *
stmt_assign(char *name, struct expr *expr, int is_const)
	{
	struct stmt *s = stmt_new(ST_ASSIGN);
	s->name = name;
	s->expr = expr;
	s->is_const = is_const;
	return s;
	}

struct stmt *
stmt_return(struct argnode *args)
	{
	struct stmt *s = stmt_new(ST_RETURN);
	s->args = args;
	return s;
	}

struct stmt *
stmt_if(struct expr *cond, struct stmt *thenb, struct stmt *elseb)
	{
	struct stmt *s = stmt_new(ST_IF);
	s->expr = cond;
	s->body = thenb;
	s->else_body = elseb;
	return s;
	}

struct stmt *
stmt_while(struct expr *cond, struct stmt *body)
	{
	struct stmt *s = stmt_new(ST_WHILE);
	s->expr = cond;
	s->body = body;
	return s;
	}

struct stmt *
stmt_call(char *name, struct argnode *args)
	{
	struct stmt *s = stmt_new(ST_CALL);
	s->name = name;
	s->args = args;
	return s;
	}

struct argnode *pack_if_multi(struct argnode *args)
	{
	int n = 0;
	for (struct argnode *a = args; a; a = argnode_next(a))
		n++;
	if (n <= 1)
		return args;                 /* 0 or 1: leave unchanged */
	struct expr *lst = ev_list(args);
	return arglist_new(lst);
	}

struct expr *
ev_binop(int op, struct expr *left, struct expr *right)
	{
	struct expr *e = expr_new(EV_BINOP);
	e->op = op;
	e->left = left;
	e->right = right;
	return e;
	}

struct expr *
ev_number(int ival)
	{
	struct expr *e = expr_new(EV_NUM);
	e->val = Value_Number(ival);
	return e;
	}

struct expr *
ev_decimal(float fval)
	{
	struct expr *e = expr_new(EV_NUM);
	e->val = Value_Decimal(fval);
	return e;
	}

struct expr *
ev_word(char *s)
	{
	struct expr *e = expr_new(EV_WORD);
	e->val = Value_Word(s);
	return e;
	}

struct expr *
ev_bool(int b)
	{
	struct expr *e = expr_new(EV_BOOL);
	e->val = Value_Bool(b);
	return e;
	}

struct expr *ev_list(struct argnode *nodes)
	{
	struct expr *e = expr_new(EV_LIST);   /* new etype */
	e->args = nodes;                      /* reuse the existing argnode chain */
	return e;
	}
struct expr *
ev_var(char *name)
	{
	struct expr *e = expr_new(EV_VAR);
	e->name = name;
	return e;
	}

struct expr *
ev_call(char *name, struct argnode *args)
	{
	struct expr *e = expr_new(EV_CALL);
	e->name = name;
	e->args = args;
	return e;
	}

/* for tree walking */
/* in ast.c — one-liners */
struct expr *expr_left(const struct expr *e)
	{
	return e ? e->left  : NULL;
	}
struct expr *expr_right(const struct expr *e)
	{
	return e ? e->right : NULL;
	}
struct stmt *stmt_body(const struct stmt *s)
	{
	return s ? s->body  : NULL;
	}
struct stmt *stmt_else(const struct stmt *s)
	{
	return s ? s->else_body : NULL;
	}
struct stmt *stmt_next(const struct stmt *s)
	{
	return s ? s->next  : NULL;
	}

void
free_expr(struct expr *e)
	{
	if (!e) return;   /* WARN: args chain not freed */
	free_expr(expr_left(e));
	free_expr(expr_right(e));
	/* free args list if present */
	free(e->name);
	Value_Free(e->val);
	free(e);
	}

void
free_stmts(struct stmt *s)
	{
	while (s)
		{
		/* WARN: args not freed on ST_CALL / ST_RETURN */
		struct stmt *next = stmt_next(s);
		free_expr(s->expr);
		free_stmts(stmt_body(s));
		free_stmts(stmt_else(s));
		free(s->name);
		free(s);
		s = next;
		}
	}

void
print_expr(struct expr *e)
	{
	if (!e)
		return;
	printf("(");
	switch (e->etype)
		{
		case EV_NUM:
		case EV_WORD:
		case EV_BOOL:
			Value_Print(&e->val);
			break;
		case EV_VAR:
			printf("%s", e->name ? e->name : "?");
			break;
		case EV_BINOP:
			print_expr(expr_left(e));
			printf(" %d ", e->op);          /* or a name for the op */
			print_expr(expr_right(e));
			break;
		case EV_CALL:
			printf("%s", e->name ? e->name : "?");
			/* walk e->args if you have an arg printer */
			break;
		default:
			printf("?");
			break;
		}
	printf(")");
	}

void
print_stmts(struct stmt *s)
	{
	while (s)
		{
		struct stmt *next = stmt_next(s);
		printf("[");
		switch (s->stype)
			{
			case ST_ASSIGN:
				printf("assign %s = ", s->name ? s->name : "?");
				print_expr(s->expr);
				break;
			/* case ST_PRINT:
			    printf("print ");
			    print_expr(s->expr);
			    break;*/
			case ST_RETURN:
				printf("return ");
				print_expr(s->expr);
				break;
			case ST_IF:
				printf("if ");
				print_expr(s->expr);
				printf(" then ");
				print_stmts(stmt_body(s));
				if (stmt_else(s))
					{
					printf(" else ");
					print_stmts(stmt_else(s));
					}
				break;
			case ST_WHILE:
				printf("while ");
				print_expr(s->expr);
				printf(" ");
				print_stmts(stmt_body(s));
				break;
			case ST_CALL:
				printf("call %s", s->name ? s->name : "?");
				break;
			case ST_BLOCK:
				printf("block ");
				print_stmts(stmt_body(s));
				break;
			default:
				printf("stmt %d", s->stype);
				break;
			}
		printf("]\n");
		s = next;
		}
	}
