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
    struct value v, c;

    /* ---------- creators + Is* / As* ---------- */

    /* empty */
    v = Value_Empty();
    check("empty type", Value_Type(v) == T_EMPTY);
    check("empty IsEmpty", Value_IsEmpty(v));
    check("empty not list", !Value_IsList(v));
    check("empty not word", !Value_IsWord(v));
    check("empty not number", !Value_IsNumber(v));
    check("empty not decimal", !Value_IsDecimal(v));
    check("empty AsList is NULL", Value_AsList(v) == NULL);
    check("empty AsWord is NULL", Value_AsWord(v) == NULL);
    check("empty AsCStr is empty string", strcmp(Value_AsCStr(v), "") == 0);
    Value_Free(v);          /* no-op for simple types, but safe */

    /* number */
    v = Value_Number(42);
    check("number type", Value_Type(v) == T_NUMBER);
    check("number IsNumber", Value_IsNumber(v));
    check("number not empty", !Value_IsEmpty(v));
    check("number AsNumber", Value_AsNumber(v) == 42);
    check("number AsDecimal is 0", Value_AsDecimal(v) == 0.0);
    Value_Free(v);

    /* decimal */
    v = Value_Decimal(3.14);
    check("decimal type", Value_Type(v) == T_DECIMAL);
    check("decimal IsDecimal", Value_IsDecimal(v));
    check("decimal AsDecimal", Value_AsDecimal(v) == 3.14);
    check("decimal AsNumber is 0", Value_AsNumber(v) == 0);
    Value_Free(v);

    /* bool */
    v = Value_Bool(1);
    check("bool type", Value_Type(v) == T_BOOL);
	check("bool true", Value_IsTrue(v));
	check("bool ival", v.ival == 1);
    Value_Free(v);

    v = Value_Bool(false);
	check("bool false", !Value_IsTrue(v));
	check("bool ival false", v.ival == 0);
    Value_Free(v);

    /* word */
    v = Value_Word("hello");
    check("word type", Value_Type(v) == T_WORD);
    check("word IsWord", Value_IsWord(v));
    check("word not list", !Value_IsList(v));
    check("word AsCStr", strcmp(Value_AsCStr(v), "hello") == 0);
    struct mstring *mw = Value_AsWord(v);
    check("word AsWord non-NULL", mw != NULL);
    check("word mstring len", mstring_len(mw) == 5);
    check("word mstring refcount 1", mstring_refcount(mw) == 1);
    Value_Free(v);          /* drops the mstring */

    /* empty word */
    v = Value_Word("");
    check("empty word type", Value_Type(v) == T_WORD);
    check("empty word AsCStr", strcmp(Value_AsCStr(v), "") == 0);
    check("empty word len 0", mstring_len(Value_AsWord(v)) == 0);
    Value_Free(v);

    /* null word becomes empty */
    v = Value_Word(NULL);
    check("null word is empty", Value_Type(v) == T_EMPTY);
    Value_Free(v);

    /* list of two words */
    {
        struct list *lst = list_new();
        list_append(Value_Word("one"), lst);
        list_append(Value_Word("two"), lst);
		struct value w1 = Value_Word("one");
struct value w2 = Value_Word("two");
list_append(w1, lst);
list_append(w2, lst);
Value_Free(w1);          /* list holds the copies */
Value_Free(w2);
v = Value_List(lst);
        v = Value_List(lst);                /* increments list refcount */
        check("list type", Value_Type(v) == T_LIST);
        check("list IsList", Value_IsList(v));
        check("list not word", !Value_IsWord(v));
        struct list *got = Value_AsList(v);
        check("list AsList non-NULL", got != NULL);
        check("list length 2", list_length(got) == 2);
        check("list head non-NULL", list_head(got) != NULL);
        check("list refcount after Value_List", list_get_refcount(got) >= 1);
        Value_Free(v);                      /* decrements; list may still be live */
        list_free(lst);                     /* explicit free of the original */
    }

    /* ---------- Value_Copy / Value_Free ---------- */

    /* simple types: copy is bitwise, free is no-op */
    v = Value_Number(99);
    c = Value_Copy(v);
    check("copy number type", Value_Type(c) == T_NUMBER);
    check("copy number value", Value_AsNumber(c) == 99);
    Value_Free(c);
    Value_Free(v);

    /* word: copy shares the mstring and raises refcount */
    v = Value_Word("shared");
    struct mstring *orig = Value_AsWord(v);
    check("word before copy refcount 1", mstring_refcount(orig) == 1);
    c = Value_Copy(v);
    check("copy word type", Value_Type(c) == T_WORD);
    check("copy word same mstring", Value_AsWord(c) == orig);
    check("copy word refcount 2", mstring_refcount(orig) == 2);
    check("copy word content", strcmp(Value_AsCStr(c), "shared") == 0);
    Value_Free(c);                          /* back to 1 */
    check("after free copy refcount 1", mstring_refcount(orig) == 1);
    check("original still valid", strcmp(Value_AsCStr(v), "shared") == 0);
    Value_Free(v);                          /* final free */

    /* list: copy shares the list and raises refcount */
    {
        struct list *lst = list_new();
        list_append(Value_Word("a"), lst);
        list_append(Value_Word("b"), lst);
        v = Value_List(lst);
        int rc0 = list_get_refcount(Value_AsList(v));
        c = Value_Copy(v);
        check("copy list type", Value_Type(c) == T_LIST);
        check("copy list same pointer", Value_AsList(c) == Value_AsList(v));
        check("copy list refcount raised",
              list_get_refcount(Value_AsList(v)) == rc0 + 1);
        Value_Free(c);
        check("after free copy list refcount restored",
              list_get_refcount(Value_AsList(v)) == rc0);
        Value_Free(v);
        list_free(lst);
    }

    /* mismatched As* return safe defaults */
    v = Value_Number(7);
    check("number AsWord is NULL", Value_AsWord(v) == NULL);
    check("number AsList is NULL", Value_AsList(v) == NULL);
    check("number AsCStr is empty", strcmp(Value_AsCStr(v), "") == 0);
    Value_Free(v);

    printf("%d failures\n", fails);
    return fails ? 1 : 0;
}
