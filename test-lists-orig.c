/* test-value.c */
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
	struct list *s, *n, *p;
	struct value v, w;
	s = n = p = NULL;
    /* ---------- creators + Is* / As* ---------- */

    /* empty */
    s = list_new();
    check("empty list", !list_OK(s));

	/* two elements */
	v = Value_Word("v");
	w = Value_Word("w");
 	list_append(v,s);
 	list_append(w,s);
    check("2-element list", !list_OK(s));
	print_list(s);

    
    printf("%d failures\n", fails);
    return fails ? 1 : 0;
}

