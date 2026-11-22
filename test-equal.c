/* test-equal.c */
#include "value.h"
#include "meanslist.h"
#include "mstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void
test_scalars(void)
{
    struct value a, b;

    /* empty */
    a = Value_Empty();
    b = Value_Empty();
    check("empty == empty", Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    /* numbers */
    a = Value_Number(42);
    b = Value_Number(42);
    check("number equal", Value_Equal(a, b));
    Value_Free(b);
    b = Value_Number(43);
    check("number unequal", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    /* negatives */
    a = Value_Number(-5);
    b = Value_Number(-5);
    check("negative equal", Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    /* decimals */
    a = Value_Decimal(3.14);
    b = Value_Decimal(3.14);
    check("decimal equal", Value_Equal(a, b));
    Value_Free(b);
    b = Value_Decimal(3.15);
    check("decimal unequal", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    /* bools */
    a = Value_Bool(1);
    b = Value_Bool(1);
    check("true == true", Value_Equal(a, b));
    Value_Free(b);
    b = Value_Bool(0);
    check("true != false", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    /* different types */
    a = Value_Number(1);
    b = Value_Bool(1);
    check("number != bool", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    a = Value_Number(0);
    b = Value_Empty();
    check("number != empty", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);
}

static void
test_words(void)
{
    struct value a, b;

    a = Value_Word("hello");
    b = Value_Word("hello");
    check("word equal", Value_Equal(a, b));
    Value_Free(b);
    b = Value_Word("Hello");
    check("word case sensitive", !Value_Equal(a, b));
    Value_Free(b);
    b = Value_Word("hello!");
    check("word length differs", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    a = Value_Word("");
    b = Value_Word("");
    check("empty words equal", Value_Equal(a, b));
    Value_Free(b);
    b = Value_Word("x");
    check("empty != non-empty", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);

    /* null word is empty */
    a = Value_Word(NULL);
    b = Value_Empty();
    check("null word == empty", Value_Equal(a, b));
    Value_Free(a); Value_Free(b);
}

static void
test_lists(void)
{
    struct value a, b;
    struct list *la, *lb;

    /* empty lists */
    la = list_new();
    lb = list_new();
    a = Value_List(la);
    b = Value_List(lb);
    check("empty lists equal", Value_Equal(a, b));
    Value_Free(a); Value_Free(b);
    list_decref(la); list_decref(lb);

    /* same content, different objects */
    la = list_new();
    lb = list_new();
    {
        struct value w = Value_Word("x");
        list_append(w, la);
        list_append(w, lb);
        Value_Free(w);
    }
    a = Value_List(la);
    b = Value_List(lb);
    check("lists same content", Value_Equal(a, b));
    Value_Free(a); Value_Free(b);
    list_decref(la); list_decref(lb);

    /* different order */
    la = list_new();
    lb = list_new();
    {
        struct value x = Value_Word("x");
        struct value y = Value_Word("y");
        list_append(x, la); list_append(y, la);
        list_append(y, lb); list_append(x, lb);
        Value_Free(x); Value_Free(y);
    }
    a = Value_List(la);
    b = Value_List(lb);
    check("lists different order", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);
    list_decref(la); list_decref(lb);

    /* different length */
    la = list_new();
    lb = list_new();
    {
        struct value x = Value_Word("x");
        list_append(x, la);
        list_append(x, la);
        list_append(x, lb);
        Value_Free(x);
    }
    a = Value_List(la);
    b = Value_List(lb);
    check("lists different length", !Value_Equal(a, b));
    Value_Free(a); Value_Free(b);
    list_decref(la); list_decref(lb);

    /* nested */
    {
        struct list *inner_a = list_new();
        struct list *inner_b = list_new();
        struct value w = Value_Word("n");
        list_append(w, inner_a);
        list_append(w, inner_b);
        Value_Free(w);

        la = list_new();
        lb = list_new();
        struct value ia = Value_List(inner_a);
        struct value ib = Value_List(inner_b);
        list_append(ia, la);
        list_append(ib, lb);
        Value_Free(ia); Value_Free(ib);
        list_decref(inner_a); list_decref(inner_b);

        a = Value_List(la);
        b = Value_List(lb);
        check("nested lists equal", Value_Equal(a, b));
        Value_Free(a); Value_Free(b);
        list_decref(la); list_decref(lb);
    }

    /* list vs non-list */
	 {
        struct list *lst = list_new();
        a = Value_List(lst);
        b = Value_Number(0);
        check("list != number", !Value_Equal(a, b));
        Value_Free(a);
        Value_Free(b);
        list_decref(lst);
    }
}

int
main(void)
{
    test_scalars();
    test_words();
    test_lists();
    printf("%d failures\n", fails);
    return fails ? 1 : 0;
}
