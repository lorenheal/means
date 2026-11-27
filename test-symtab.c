/* test-symtab.c — unit tests for symtab.c/h */
/* #define BISON_FREE_TESTING */

#include "symtab.h"
#include "value.h"
#include "func.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fails;
unsigned int debug_mask = 0;
/* extern unsigned int error_count = 0; */
extern unsigned int yylineno;

static void
check(const char *name, int cond)
{
	if (!cond) {
		printf("FAIL %s\n", name);
		fails++;
	} else {
		printf("ok   %s\n", name);
	}
}

int
main(void)
{
	struct scope *g, *inner, *deep;
	struct value v, got;
	struct sym *s;
/*	struct func *f; */

	/* ---------- scope_new / scope_free ---------- */
	g = scope_new("global", NULL);
	check("scope_new: not NULL", g != NULL);
	check("scope_new: name", g && g->name && strcmp(g->name, "global") == 0);
	check("scope_new: outer NULL", g && g->outer == NULL);
	check("scope_new: empty syms", g && g->syms == NULL);
	scope_free(g);

	/* ---------- sym_set_value / sym_find / sym_value ---------- */
	g = scope_new("global", NULL);
	v = Value_Number(42);
	sym_set_value("answer", v, g);
	Value_Free(v);

	s = sym_find("answer", g);
	check("set/find: found", s != NULL);
	check("set/find: name", s && strcmp(s->name, "answer") == 0);
	got = sym_value(s);
	check("set/find: value 42", Value_Type(got) == T_NUMBER && Value_AsNumber(got) == 42);
	Value_Free(got);

	s = sym_find("missing", g);
	check("find missing: NULL", s == NULL);

	s = sym_find_in_scope("answer", g);
	check("find_in_scope: found", s != NULL);
	s = sym_find_in_scope("nope", g);
	check("find_in_scope missing: NULL", s == NULL);

	/* overwrite */
	v = Value_Number(99);
	sym_set_value("answer", v, g);
	Value_Free(v);
	s = sym_find("answer", g);
	got = sym_value(s);
	check("overwrite: 99", Value_AsNumber(got) == 99);
	Value_Free(got);

	/* ---------- nested scopes ---------- */
	inner = scope_new("inner", g);
	deep  = scope_new("deep", inner);

	v = Value_Word("outer");
	sym_set_value("name", v, g);
	Value_Free(v);

	v = Value_Word("inner");
	sym_set_value("name", v, inner);
	Value_Free(v);

	s = sym_find("name", deep);
	check("nested find: walks outer", s != NULL);
	got = sym_value(s);
	check("nested find: sees inner", Value_IsWord(got) &&
	      strcmp(Value_AsCStr(got), "inner") == 0);
	Value_Free(got);

	s = sym_find_in_scope("name", deep);
	check("find_in_scope deep: NULL", s == NULL);

	s = sym_find("answer", deep);
	check("nested finds global", s != NULL && Value_AsNumber(sym_value(s)) == 99);

	/* ---------- functions ---------- */
	struct func *f = func_create("double", g);
	sym_define_function("double", f, g);
	check("define_function: finds", sym_find_function("double", g) == f);
	check("define_function: walks outer", sym_find_function("double", deep) == f);
	check("define_function missing: NULL", sym_find_function("nope", g) == NULL);
	/* symbol type is T_FUNC */
	s = sym_find("double", g);
	check("function symbol type", s && s->vtype == T_FUNC);

	/* ---------- null safety ---------- */
	check("find NULL name: NULL", sym_find(NULL, g) == NULL);
	check("find NULL scope: NULL", sym_find("x", NULL) == NULL);
	check("find_in_scope NULL: NULL", sym_find_in_scope(NULL, g) == NULL);

	/* cleanup */
	scope_free(deep);
	scope_free(inner);
	scope_free(g);
	/* func_free(f); */

	printf("%d failures\n", fails);
	return fails ? 1 : 0;
}
