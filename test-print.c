/* test-print.c */
#include "value.h"
#include "meanslist.h"
#include "mstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

/* Capture Value_Print output and return its count.  Caller frees *out. */
static int
capture_and_print(struct value *v, char **out)
{
    char tmp[] = "/tmp/means-print-XXXXXX";
    int fd = mkstemp(tmp);
    if (fd < 0) return -1;
    FILE *old = stdout;
    stdout = fdopen(fd, "w");
    if (!stdout) { close(fd); unlink(tmp); stdout = old; return -1; }
    int n = Value_Print(v);
    fflush(stdout);
    fclose(stdout);
    stdout = old;

    FILE *f = fopen(tmp, "r");
    if (!f) { unlink(tmp); return -1; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    *out = calloc(1, len + 1);
    if (*out) fread(*out, 1, len, f);
    fclose(f);
    unlink(tmp);
    return n;
}

static void
test_simple(void)
{
    char *out = NULL;
    struct value v;
    int n;

    n = Value_Print(NULL);
    check("null pointer returns 0", n == 0);

    v = Value_Empty();
    n = capture_and_print(&v, &out);
    check("empty prints nothing", out && out[0] == '\0');
    check("empty returns 0", n == 0);
    free(out); out = NULL;
    Value_Free(v);

    v = Value_Number(42);
    n = capture_and_print(&v, &out);
    check("number prints 42", out && strcmp(out, "42") == 0);
    check("number returns 1", n == 1);
    free(out); out = NULL;
    Value_Free(v);

    v = Value_Number(-7);
    n = capture_and_print(&v, &out);
    check("negative prints -7", out && strcmp(out, "-7") == 0);
    free(out); out = NULL;
    Value_Free(v);

    v = Value_Decimal(3.14);
    n = capture_and_print(&v, &out);
    check("decimal prints", out && strstr(out, "3.14") != NULL);
    check("decimal returns 1", n == 1);
    free(out); out = NULL;
    Value_Free(v);

    v = Value_Bool(1);
    n = capture_and_print(&v, &out);
    check("true prints true", out && strcmp(out, "true") == 0);
    free(out); out = NULL;
    Value_Free(v);

    v = Value_Bool(0);
    n = capture_and_print(&v, &out);
    check("false prints false", out && strcmp(out, "false") == 0);
    free(out); out = NULL;
    Value_Free(v);

    v = Value_Word("hello");
    n = capture_and_print(&v, &out);
    check("word prints hello", out && strcmp(out, "hello") == 0);
    check("word returns 1", n == 1);
    free(out); out = NULL;
    Value_Free(v);

    v = Value_Word("");
    n = capture_and_print(&v, &out);
    check("empty word prints nothing", out && out[0] == '\0');
    check("empty word returns 0", n == 0);
    free(out); out = NULL;
    Value_Free(v);
}

static void
test_lists(void)
{
    char *out = NULL;
    struct list *lst;
    struct value v;
    int n;

    /* empty list */
    lst = list_new();
    v = Value_List(lst);
    n = capture_and_print(&v, &out);
    check("empty list prints nothing", out && out[0] == '\0');
    check("empty list returns 0", n == 0);
    free(out); out = NULL;
    Value_Free(v);
    list_decref(lst);

    /* single element */
    lst = list_new();
    {
        struct value w = Value_Word("only");
        list_append(w, lst);
        Value_Free(w);
    }
    v = Value_List(lst);
    n = capture_and_print(&v, &out);
    check("single prints only", out && strcmp(out, "only") == 0);
    free(out); out = NULL;
    Value_Free(v);
    list_decref(lst);

    /* three words */
    lst = list_new();
    {
        struct value a = Value_Word("a");
        struct value b = Value_Word("b");
        struct value c = Value_Word("c");
        list_append(a, lst);
        list_append(b, lst);
        list_append(c, lst);
        Value_Free(a); Value_Free(b); Value_Free(c);
    }
    v = Value_List(lst);
    n = capture_and_print(&v, &out);
    check("three words", out && strcmp(out, "a b c") == 0);
    free(out); out = NULL;
    Value_Free(v);
    list_decref(lst);

    /* empty elements do not produce extra spaces */
    lst = list_new();
    {
        struct value a = Value_Word("a");
        struct value e = Value_Word("");
        struct value b = Value_Word("b");
        list_append(a, lst);
        list_append(e, lst);
        list_append(b, lst);
        Value_Free(a); Value_Free(e); Value_Free(b);
    }
    v = Value_List(lst);
    n = capture_and_print(&v, &out);
    check("empty element skipped for spacing",
          out && strcmp(out, "a b") == 0);
    free(out); out = NULL;
    Value_Free(v);
    list_decref(lst);

    /* mixed types */
    lst = list_new();
    {
        struct value num = Value_Number(10);
        struct value w = Value_Word("items");
        struct value b = Value_Bool(1);
        list_append(num, lst);
        list_append(w, lst);
        list_append(b, lst);
        Value_Free(num); Value_Free(w); Value_Free(b);
    }
    v = Value_List(lst);
    n = capture_and_print(&v, &out);
    check("mixed types", out && strcmp(out, "10 items true") == 0);
    free(out); out = NULL;
    Value_Free(v);
    list_decref(lst);

    /* nested list */
    {
        struct list *inner = list_new();
        struct value x = Value_Word("x");
        struct value y = Value_Word("y");
        list_append(x, inner);
        list_append(y, inner);
        Value_Free(x); Value_Free(y);

        lst = list_new();
        struct value outer_w = Value_Word("outer");
        struct value inner_v = Value_List(inner);
        list_append(outer_w, lst);
        list_append(inner_v, lst);
        Value_Free(outer_w);
        Value_Free(inner_v);
        list_decref(inner);

        v = Value_List(lst);
        n = capture_and_print(&v, &out);
        check("nested list", out && strcmp(out, "outer x y") == 0);
        free(out); out = NULL;
        Value_Free(v);
        list_decref(lst);
    }
}

int
main(void)
{
    test_simple();
    test_lists();
    printf("%d failures\n", fails);
    return fails ? 1 : 0;
}