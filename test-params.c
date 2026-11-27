/* test-params.c — unit tests for params.c/h */
#include "params.h"
#include "func.h"
#include "value.h"
#include "symtab.h"
#include "meanslist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fails;
unsigned int debug_mask = 0;
/* unsigned int error_count = 0;*/

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

/* helper: count params in a linked list */
static int
count_params(struct param *p)
{
	int n = 0;
	while (p) { n++; p = p->next; }
	return n;
}

/* helper: build a minimal func with the given param list */
static struct func *
make_func(const char *name, struct param *params)
{
	struct func *f = func_create(name, NULL);
	f->params = params;
	f->param_count = count_params(params);
	return f;
}

int
main(void)
{
	struct param *p, *q, *list;
	struct value v, w;
	struct func *f;
	struct scope *sc;
	struct value argv[4];
	struct sym *s;
	struct value got;
	match_result mr;

	/* ---------- param_new ---------- */
	v = Value_Number(0);
	p = param_new("n", v);
	check("param_new: not NULL", p != NULL);
	check("param_new: name", p && strcmp(p->name, "n") == 0);
	check("param_new: vtype NUMBER", p && p->vtype == T_NUMBER);
	check("param_new: required 0", p && p->required == 0);
	check("param_new: is_rest 0", p && p->is_rest == 0);
	check("param_new: next NULL", p && p->next == NULL);
	Value_Free(v);
	param_free(p);

	/* ---------- param_append ---------- */
	v = Value_Number(1);
	w = Value_Word("hi");
	list = param_append(NULL, "a", v);
	list = param_append(list, "b", w);
	check("append: count 2", count_params(list) == 2);
	check("append: first name a", list && strcmp(list->name, "a") == 0);
	check("append: second name b", list && list->next &&
	      strcmp(list->next->name, "b") == 0);
	check("append: second vtype WORD", list && list->next &&
	      list->next->vtype == T_WORD);
	Value_Free(v);
	Value_Free(w);
	param_free(list);

	/* ---------- param_link ---------- */
	v = Value_Number(10);
	p = param_new("x", v);
	q = param_new("y", v);
	list = param_link(NULL, p);
	list = param_link(list, q);
	check("link: count 2", count_params(list) == 2);
	check("link: order x then y", list && list->next &&
	      strcmp(list->name, "x") == 0 &&
	      strcmp(list->next->name, "y") == 0);
	Value_Free(v);
	param_free(list);

	/* ---------- params_match: exact ---------- */
	v = Value_Number(0);
	p = param_new("n", v);
	p->required = 1;
	f = make_func("inc", p);
	argv[0] = Value_Number(5);
	mr = params_match(f, argv, 1);
	check("match exact: OK", mr == MATCH_OK);
	Value_Free(argv[0]);

	/* too few */
	mr = params_match(f, NULL, 0);
	check("match too few: TOO_FEW", mr == MATCH_TOO_FEW);

	/* too many (not rest) */
	argv[0] = Value_Number(1);
	argv[1] = Value_Number(2);
	mr = params_match(f, argv, 2);
	check("match too many: TOO_MANY", mr == MATCH_TOO_MANY);
	Value_Free(argv[0]);
	Value_Free(argv[1]);

	/* type mismatch */
	argv[0] = Value_Word("nope");
	mr = params_match(f, argv, 1);
	check("match type: TYPE_MISMATCH", mr == MATCH_TYPE_MISMATCH);
	Value_Free(argv[0]);

	func_free(f);   /* frees params */

	/* ---------- params_match: optional ---------- */
	v = Value_Number(0);
	p = param_new("n", v);   /* required = 0 */
	f = make_func("opt", p);
	mr = params_match(f, NULL, 0);
	check("optional zero args: OK", mr == MATCH_OK);
	func_free(f);
	Value_Free(v);

	/* ---------- params_match: rest ---------- */
	v = Value_List(list_new());
	p = param_new("args", v);
	p->is_rest = 1;
	f = make_func("show", p);
	func_set_is_rest(f, 1);
	argv[0] = Value_Number(1);
	argv[1] = Value_Number(2);
	mr = params_match(f, argv, 2);
	check("rest match: OK", mr == MATCH_OK);
	Value_Free(argv[0]);
	Value_Free(argv[1]);
	func_free(f);
	Value_Free(v);

	/* ---------- bind_fixed_params ---------- */
	sc = scope_new("local", NULL);
	v = Value_Number(0);
	p = param_new("n", v);
	p->required = 1;
	f = make_func("bind", p);

	/* build argument list */
	{
	struct list *lst = list_new();
	struct value arg = Value_Number(7);
	list_append(arg, lst);
	Value_Free(arg);
	struct value arguments = Value_List(lst);
	bind_fixed_params(f, arguments, sc);
	Value_Free(arguments);
	}

	s = sym_find("n", sc);
	check("bind: symbol created", s != NULL);
	got = sym_value(s);
	check("bind: value 7", Value_Type(got) == T_NUMBER &&
	      Value_AsNumber(got) == 7);
	Value_Free(got);

	func_free(f);
	scope_free(sc);
	Value_Free(v);

	/* ---------- arglist helpers ---------- */
	struct argnode *a = arglist_new(NULL);
	check("arglist_new: not NULL", a != NULL);
	a = arglist_append(NULL, a);
	check("arglist_append: still not NULL", a != NULL);
	check("arglist_append: has next", a && a->next != NULL);
	check("argnode_next: works", argnode_next(a) == a->next);
	arglist_free(a);

	printf("%d failures\n", fails);
	return fails ? 1 : 0;
}
