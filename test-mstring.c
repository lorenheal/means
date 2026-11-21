/* test-value.c */
#include "value.h"
#include "meanslist.h"
#include "mstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fails;
unsigned int debug_mask=0;

static void
check(const char *name, int cond)
	{
	if (!cond)
		{
		printf("FAIL %s\n", name);
		fails++;
		}
	else
		printf("ok   %s\n", name);
	}

int
main(void)
	{
	struct mstring *m, *n;


	/* empty */
	m = mstring_new("");
	check("empty string length 0", mstring_len(m) == 0);

	/* append */
	mstring_append("0123456789",m);
	check("append to string of length 0", mstring_len(m) == 10);
	mstring_free(m);

	/* combine two words into one append version */
	m = mstring_new("one(m)");
	n = mstring_new("two(n)");
	mstring_append(mstring_cstr(n), m);
	check("append strings", !strcmp("one(m)two(n)", mstring_cstr(m)));
	/*v = Value_Word(mstring_cstr((m)));	Value_Print(&v);	*/
	mstring_free(n);
	mstring_free(m);
	
	/* combine two words into one, insert version*/
	m = mstring_new("one(m)");
	n = mstring_new("two(n)");
	mstring_insert(mstring_cstr(n), m);
	check("insert strings", !strcmp("two(n)one(m)", mstring_cstr(m)));
	mstring_free(n);
	mstring_free(m);
		/* --- reference counting --- */
	{
	struct mstring *a, *b;
	/* new starts at 1 */
	a = mstring_new("shared");
	check("new refcount == 1", mstring_refcount(a) == 1);

	/* incref raises it; both aliases see the same object */
	b = mstring_incref(a);
	check("incref raises to 2", mstring_refcount(a) == 2);
	check("incref returns same pointer", a == b);

	/* one decref leaves it alive */
	mstring_decref(b);
	check("after one decref still 1", mstring_refcount(a) == 1);
	check("data still intact", !strcmp(mstring_cstr(a), "shared"));

	/* append while uniquely owned is fine */
	mstring_append("!", a);
	check("append after unique", !strcmp(mstring_cstr(a), "shared!"));
	check("refcount unchanged by append", mstring_refcount(a) == 1);

	/* final free drops to 0 and releases */
	mstring_free(a);
	/* a is now dangling; do not use it.  If you have a debug
	   build that poisons freed memory you can check that here. */
	}

	/* shared mutation is visible to every alias until the last free */
	{
	struct mstring *a = mstring_new("base");
	struct mstring *b = mstring_incref(a);
	mstring_append("-x", a);
	check("mutation visible through other alias",
	      !strcmp(mstring_cstr(b), "base-x"));
	check("both still live", mstring_refcount(a) == 2);
	mstring_free(a);
	check("one free leaves the other", mstring_refcount(b) == 1);
	mstring_free(b);          /* last reference */
	}

	/* find one string in another (0 based) */
	{
	char *ms = "0haystack9needle7890haystack9";
	char *ns = "needle";
	m = mstring_new(ms);
	n = mstring_new(ns);
	/* printf("%s pos %d in %s", ns, mstring_find(n,m), ms); */
	check("find needle in haystack", (10 == mstring_find(n,m)));
	mstring_free(m);
	mstring_free(n);
	}

	/* split */
	{
	char *whole = ("0123four8901234five90123");
	m = mstring_new(whole);/* owned */
	n = mstring_split(m,10);/* owned m, n */
	const char *ms = mstring_cstr(m); 
	const char *ns = mstring_cstr(n); 
	int nr = mstring_refcount(n);
	int mr = mstring_refcount(m);
	check("split refcounts", (nr == 1) && (mr == 1));
	/*	printf("%s  %s\n", ns,  ms); */
	mstring_append(ns,m);
	ms = mstring_cstr(m);
	/* printf("%s\n", ms); */
	check("append after split", !strcmp(ms,whole));
	mstring_free(m);
	mstring_free(n);
	}

	/* rest */
		/* rest */
	{
	char *whole = "0123four8901234five90123";
	m = mstring_new(whole);                 /* owned */
	n = mstring_rest(m, 10);                /* owned; original untouched */
	const char *ms = mstring_cstr(m);
	const char *ns = mstring_cstr(n);
	check("rest refcounts",
	      mstring_refcount(m) == 1 && mstring_refcount(n) == 1);
	check("rest original unchanged", !strcmp(ms, whole));
	check("rest tail", !strcmp(ns, whole + 10));
	/* rest past end yields empty */
	struct mstring *empty = mstring_rest(m, 1000);
	check("rest past end empty", mstring_len(empty) == 0);
	mstring_free(empty);
	/* rest at 0 is a full copy */
	struct mstring *copy = mstring_rest(m, 0);
	check("rest at 0 is copy", !strcmp(mstring_cstr(copy), whole));
	check("rest at 0 is distinct", copy != m);
	mstring_free(copy);
	mstring_free(m);
	mstring_free(n);
	}
	
	printf("%d failures\n", fails);
	return fails ? 1 : 0;
	}
