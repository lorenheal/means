/* test-list.c */
#include "value.h"
#include "meanslist.h"
#include "mstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int fails;
unsigned int debug_mask = 0;

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
	struct list *s, *n, *r;
	struct value v, w, x, got;
	struct listnode *node;

	/* ---------- list_new ---------- */
	s = list_new();
	check("new: OK", !list_OK(s));
	check("new: length 0", list_length(s) == 0);
	check("new: isempty", list_isempty(s));
	check("new: refcount 1", list_get_refcount(s) == 1);

	/* ---------- list_append / push / insert ---------- */
	v = Value_Word("a");
	w = Value_Word("b");
	x = Value_Word("c");
	list_append(v, s);
	list_append(w, s);
	list_push(x, s);
	check("append+push: OK", !list_OK(s));
	check("append+push: length 3", list_length(s) == 3);
	check("append+push: !isempty", !list_isempty(s));
	/* push puts at head: x, a, b */
	check("push at head: first is c", Value_Equal(list_first(s), x));
	check("append at tail: last is b", Value_Equal(list_last(s), w));
	/* insert at head (same as push in most impls) */
	list_insert(v, s);
	check("insert: length 4", list_length(s) == 4);
	check("insert: OK", !list_OK(s));

	/* ---------- list_length ---------- */
	check("length matches", list_length(s) == 4);

	/* ---------- list_first / list_last on empty ---------- */
	r = list_new();
	got = list_first(r);
	check("first on empty: T_EMPTY", Value_Type(got) == T_EMPTY);
	got = list_last(r);
	check("last on empty: T_EMPTY", Value_Type(got) == T_EMPTY);
	list_free(r);

	/* ---------- list_pop ---------- */
	got = list_pop(s);
 /*	Value_Print(&got); printf(" vs "); Value_Print(&w); printf("\n");*/
	check("pop: returns first (a)", Value_Equal(got, v));
	Value_Free(got);
	check("pop: length 3", list_length(s) == 3);
	check("pop: OK", !list_OK(s));
	/* pop until empty */
	while (!list_isempty(s))
		{
		got = list_pop(s);
		Value_Free(got);
		}
		
	check("pop to empty: length 0", list_length(s) == 0);
	check("pop to empty: isempty", list_isempty(s));
	check("pop to empty: OK", !list_OK(s));
	got = list_pop(s);
	check("pop on empty: T_EMPTY", Value_Type(got) == T_EMPTY);

	/* ---------- list_split ---------- */
	/* build: a, b, c */
	list_append(v, s);
	list_append(w, s);
	list_append(x, s);
	check("pre-split: length 3", list_length(s) == 3);
	n = list_split(s);
	check("split: orig length 1", list_length(s) == 1);
	check("split: orig first is a", Value_Equal(list_first(s), v));
	check("split: rest length 2", list_length(n) == 2);
	check("split: rest first is b", Value_Equal(list_first(n), w));
	check("split: rest last is c", Value_Equal(list_last(n), x));
	check("split: both OK", !list_OK(s) && !list_OK(n));
	list_free(n);
	list_free(s);

	/* ---------- list_rest ---------- */
	s = list_new();
	list_append(v, s);
	list_append(w, s);
	list_append(x, s);
	struct list *r1 = list_rest(s);
	check("rest: orig unchanged length 3", list_length(s) == 3);
	check("rest: new length 2", list_length(r1) == 2);
	check("rest: new first is b", Value_Equal(list_first(r1), w));
	check("rest: new last is c", Value_Equal(list_last(r1), x));
	check("rest: both OK", !list_OK(s) && !list_OK(r1));
	/* rest of single-element list → empty */
	struct list *r2 = list_rest(r1);
	struct list *r3 = list_rest(r2);
	check("rest of 1: length 0", list_length(r3) == 0);
	check("rest of 1: isempty", list_isempty(r3));
	list_free(r3);
	list_free(r2);
	list_free(r1);
	list_free(s);

	/* ---------- list_copy ---------- */
	s = list_new();
	list_append(v, s);
	list_append(w, s);
	n = list_copy(s);
	check("copy: length equal", list_length(n) == list_length(s));
	check("copy: equal content", list_equal(s, n));
	check("copy: both OK", !list_OK(s) && !list_OK(n));
	/* independence: mutate copy, original unchanged */
	list_pop(n);
	check("copy: independent after pop", list_length(s) == 2);
	check("copy: orig still OK", !list_OK(s));
	list_free(n);
	list_free(s);

	/* ---------- list_equal ---------- */
	s = list_new();
	n = list_new();
	list_append(v, s);
	list_append(v, n);
	check("equal: same content", list_equal(s, n));
	list_append(w, n);
	check("equal: different length", !list_equal(s, n));
	list_free(s);
	list_free(n);

	/* ---------- list_find ---------- */
	s = list_new();
	list_append(v, s);
	list_append(w, s);
	list_append(x, s);
	check("find: a at 1", list_find(v, s) == 0);
	check("find: b at 2", list_find(w, s) == 1);
	check("find: c at 3", list_find(x, s) == 2);
	struct value z = Value_Word("z");
	check("find: missing is 0", list_find(z, s) == -1);
	list_free(s);

	/* ---------- node accessors ---------- */
	s = list_new();
	list_append(v, s);
	list_append(w, s);
	node = list_head(s);
	check("head: not NULL", node != NULL);
	check("head: value is a", Value_Equal(*(listnode_value(node)), v));
	check("head: listnode_val same", Value_Equal(listnode_val(node), v));
	node = listnode_next(node);
	check("next: not NULL", node != NULL);
	check("next: value is b", Value_Equal(*(listnode_value(node)), w));
	check("next: next is NULL", listnode_next(node) == NULL);
	node = list_tail(s);
	check("tail: value is b", Value_Equal(*(listnode_value(node)), w));
	/* list_next via the node-level API */
	node = list_head(s);
	check("list_next: matches listnode_next", list_next(node) == listnode_next(list_head(s)));
	list_free(s);

	/* ---------- refcount ---------- */
	s = list_new();
	check("refcount: initial 1", list_get_refcount(s) == 1);
	list_incref(s);
	check("refcount: after incref 2", list_get_refcount(s) == 2);
	list_decref(s);
	check("refcount: after decref 1", list_get_refcount(s) == 1);
	list_decref(s);
	/* at 0 the list is freed; do NOT dereference s further */
	Value_Free(v);
	Value_Free(w);
	Value_Free(x);
	Value_Free(z);

	/* ---------- list_free on empty ---------- */
	s = list_new();
	list_free(s);
	/* nothing to check after free; just verify no crash */


	printf("%d failures\n", fails);

	return fails ? 1 : 0;
}   
