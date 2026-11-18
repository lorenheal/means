/* test-value.c */
#include "value.h"
#include "meanslist.h"
#include <stdio.h>
#include <stdlib.h>

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
	struct value v;

	/* empty */
	v = Value_Empty();
	check("empty type", Value_Type(v) == T_EMPTY);
	check("empty islist", !Value_IsList(v));

	/* word */
	v = Value_Word("hello");
	check("word type", Value_Type(v) == T_WORD);
	check("word islist", !Value_IsList(v));

	/* list of two words */
	struct list *lst = list_new();
	list_append( Value_Word("one"), lst);
	list_append( Value_Word("two"), lst);
	v = Value_List(lst);
	check("list type", Value_Type(v) == T_LIST);
	check("list islist", Value_IsList(v));
	struct list *got = Value_AsList(v);
	check("list head", list_head(got) != NULL);

	printf("%d failures\n", fails);
	return fails ? 1 : 0;
	}
