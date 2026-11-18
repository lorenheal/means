#include "builtins.h"
#include "meanslist.h"
#include "mstring.h"
#include "params.h"
#include "help.h"
#include <string.h>

static struct value builtin_print(struct value *args, int argc, struct scope *);
static struct value builtin_roll(struct value *args, int argc, struct scope *scope);
static struct value builtin_first(struct value *args, int argc, struct scope *scope);
static struct value builtin_rest(struct value *args, int argc, struct scope *scope);
static struct value builtin_split(struct value *args, int argc, struct scope *scope);
static struct value builtin_length(struct value *args, int argc, struct scope *scope);
static struct value builtin_last(struct value *args, int argc, struct scope *scope);
static struct value builtin_append(struct value *args, int argc, struct scope *scope);
static struct value builtin_insert(struct value *args, int argc, struct scope *scope);
static struct value builtin_push(struct value *args, int argc, struct scope *scope);
static struct value builtin_pop(struct value *args, int argc, struct scope *scope);
static struct value builtin_join(struct value *args, int argc, struct scope *scope);
static struct value builtin_find(struct value *args, int argc, struct scope *scope);
static struct value builtin_exit(struct value *args, int argc, struct scope *scope);
static struct value builtin_read(struct value *args, int argc, struct scope *scope);

struct builtin_desc {
    const char *name;
    struct value (*fn)(struct value *args, int argc, struct scope *scope);
    int nparams;          /* 0 = unchecked */
    int types[4];         /* enough for now; widen if needed */
};

static const struct builtin_desc builtins[] = {
    { "print",  builtin_print,  0, { 0 } },
    /* list ops : */
    { "read",   builtin_read,   0, { 0 } },
    { "roll",  builtin_roll,  0, { 0 } },
    { "first",  builtin_first,  1, { T_LIST } },
    { "first",  builtin_first,  1, { T_MSTRING } },
    { "rest",   builtin_rest,   1, { T_LIST } },
    { "rest",   builtin_rest,   1, { T_MSTRING } },
    { "split",   builtin_split,   1, { T_LIST } },
    { "split",   builtin_split,   1, { T_MSTRING } },
/*    { "split",   builtin_split,   2, { T_MSTRING, T_INT } }, */
    { "last",   builtin_last,   1, { T_LIST } },
    { "last",   builtin_last,   1, { T_MSTRING } },
    { "length",   builtin_length,   1, { T_LIST } },
    { "length",   builtin_length,   1, { T_MSTRING } },
    { "append",   builtin_append,   2, { T_ANY, T_LIST } },
    { "append",   builtin_append,   2, { T_MSTRING, T_MSTRING } },
    { "insert",   builtin_insert,   2, { T_ANY, T_LIST } },
    { "insert",   builtin_insert,   2, { T_MSTRING, T_MSTRING } },
    { "push",   builtin_push,   2, { T_ANY, T_LIST } },
    { "pop",   builtin_pop,   1, { T_LIST } },
    { "find",   builtin_find,   2, { T_ANY, T_LIST } },
    { "find",   builtin_find,   2, { T_MSTRING, T_MSTRING } },
    { "join", builtin_join, 2, { T_MSTRING, T_LIST } },
    /* { "join", builtin_join, 2, { T_MSTRING, T_MSTRING } }, */
    { "exit",   builtin_exit,   1, { T_INT } },
    { "exit",   builtin_exit,   2, { T_INT, T_MSTRING } }
};

int
list_find(struct value v, const struct list *lst)
	{
	int i = 0;
	if (!lst)
		return -1;
	for (const struct listnode *n = list_head(lst); n; n = listnode_next(n), i++)
		if (value_equal(listnode_val(n), v))
			return i;
	return -1;
	}


void register_builtins(struct scope *global)
{
    int n = sizeof builtins / sizeof builtins[0];
    for (int i = 0; i < n; i++) {
        const struct builtin_desc *d = &builtins[i];
        struct func *f = calloc(1, sizeof *f);
        f->name    = (char *)d->name; 
        f->builtin = d->fn;
        diagnose(DIAG_DEBUG, DBG_BUILTIN, 0, 0,
            "reg '%s' nparams=%d", f->name, f->param_count); 
        if (DEBUG(DBG_VERBOSE)) /* skip the for loop if not verbose */
        {
            for (int i = 0; i < f->param_count; i++)
                diagnose(DIAG_DEBUG, (DBG_BUILTIN & DBG_VERBOSE), 0, 0, 
					" t%d=%d", i, f->params[i].type);
        }
        diagnose(DIAG_DEBUG, DBG_BUILTIN, 0, 0, " T_ANY=%d T_LIST=%d\n", T_ANY, T_LIST);
        params_init(f);
        for (int p = 0; p < d->nparams; p++)
            params_add(f, NULL, d->types[p]);
        sym_define_function(f->name, f, global);
    }
}

static struct value builtin_exit(struct value *args, int argc, struct scope *scope)
{
    /* FIXME add errorlevel, description */
    (void) args;
    (void) argc;
    (void) scope;
    switch (argc) {
    case 2:  break;
    case 1: break;
    default:  break;
    }
    exit(0);
}

static struct value builtin_print(struct value *args, int argc, struct scope *scope)
{
    (void)scope;
    if (DEBUG(DBG_PRINT)) printf("[print ");
    for (int i = 0; i < argc; i++) {
        if (i) printf(" ");
        value_print(&args[i]);
    }
    if (DEBUG(DBG_PRINT) > 1) printf("]");
    printf("\n");
    return Value_Empty();
}

static struct value builtin_read(struct value *args, int argc,  struct scope *scope)
{
    (void)args; (void) argc; (void)scope;          /* no arguments for now */
    char buf[1024];
    if (!fgets(buf, sizeof buf, stdin)) {
        return Value_Empty();
    }
    /* strip the trailing newline */
    size_t n = strlen(buf);
    if (n && buf[n-1] == '\n') buf[n-1] = '\0';
    struct value v;
    value_set_type(v, T_MSTRING);
    value_set_mstr(v, mstring_new(buf));
    return v;
}

static struct value builtin_roll(struct value *args, int argc, struct scope *scope)
{
    (void)scope;
    struct list *lst = list_new();
    for (int i = 0; i < argc; i++)
        list_append(args[i], lst);
    struct value ret;
    value_set_type(ret, T_LIST);
    ret.lval = lst;
    return ret;
}

static struct value builtin_first(struct value *args, int argc, struct scope *scope)
{
    struct value ret = Value_Empty();
    (void)scope; (void)argc;          /* params_match already checked */
    switch (value_type(args[0])) {
    case T_LIST:
        if (value_get_list(&args[0]))
            ret = list_first(value_get_list(&args[0]));  /* already returns an owned value */
        break;
    case T_MSTRING: {
        struct mstring *m = value_get_mstring(&args[0]);
        value_set_type(ret, T_MSTRING);
        if (m && mstring_len(m) > 0) {
            const char *s = mstring_cstr(m);
            char tmp[2] = { s ? *s : '\0', '\0' };
            if (s) *tmp = *s;
            value_set_mstr(ret, mstring_new(tmp));
        } else
            value_set_mstr(ret, mstring_new(""));
        break;
    }
    default:
        diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "first: unsupported type\n");
        break;
    }
    return ret;
}

char * str_first(const char *whole)
{
    if (!whole || whole[0] == '\0') return NULL;
    char* new_str = strdup(" ");; // 1 char + 1 null terminator
    if (new_str) {
        *new_str = *whole;
    }
    return new_str;
}

char * str_rest(const char *whole)
{
    char *ret = NULL;
    if (whole && whole[0] != '\0')
        ret = strdup(whole+1);
    return ret;
}

char * str_split(char *whole)
{
    if (!whole || whole[0] == '\0')
        return strdup("");
    char *tail = strdup(whole + 1);   /* owned copy */
    whole[1] = '\0';                  /* original now holds only the first char */
    return tail;
}

static struct value builtin_rest(struct value *args, int argc, struct scope *scope)
{
    struct value ret = Value_Empty();
    (void)scope; (void)argc;
    switch (value_type(args[0])) {
    case T_LIST:
        if (value_get_list(&args[0])) {
            value_set_type(ret, T_LIST);
            ret.lval = list_rest(value_get_list(&args[0]));
        }
        break;
    case T_MSTRING: {
        int index = 1; /* this should be passed as a parameter */
        value_set_mstr(ret, mstring_rest(value_get_mstring(&args[0]), index));
        break;
    }
    default:
        diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "rest: unsupported type\n");
        break;
    }
    return ret;
}

static struct value builtin_split(struct value *args, int argc, struct scope *scope)
{
    struct value ret = Value_Empty();
    (void)scope; (void)argc;
    switch (value_type(args[0])) {
    case T_LIST:
        value_set_type(ret, T_LIST);
        ret.lval = list_split(value_get_list(&args[0]));  /* transfers tail; original shortened */
        break;
    case T_MSTRING: {
        int index = ((argc > 1) ? (value_get_int(&args[1])) : (1));
        value_set_mstr(ret, mstring_split(value_get_mstring(&args[0]), index));
        break;
    }
    default:
        diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "split: unsupported type\n");
        break;
    }
    return ret;
}

static struct value builtin_length(struct value *args, int argc, struct scope *scope)
{
    (void)scope; (void)argc;
    struct value ret = Value_Zero();
    switch (value_type(args[0])) {
    case T_LIST:
        value_set_int(ret, list_length(value_get_list(&args[0])));
        break;
    case T_MSTRING:
        value_set_int(ret, mstring_len(value_get_mstring(&args[0])));
        break;
    default:
        diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "length: unsupported type\n");
        break;
    }
    return ret;
}

static struct value builtin_last(struct value *args, int argc, struct scope *scope)
{
    (void)scope; (void)argc;
    struct value ret = Value_Empty();
    switch (value_type(args[0])) {
    case T_LIST:
        ret = list_last(value_get_list(&args[0]));
        break;
    case T_MSTRING: {
        struct mstring *m = value_get_mstring(&args[0]);
        value_set_type(ret, T_MSTRING);
        if (m && mstring_len(m) > 0) {
            const char *s = mstring_cstr(m);
            value_set_mstr(ret, mstring_new(s + mstring_len(m) - 1));
        } else
            value_set_mstr(ret, mstring_new(""));
        break;
    }
    default:
        diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "last: unsupported type\n");
        break;
    }
    return ret;
}

static struct value builtin_append(struct value *args, int argc, struct scope *scope)
{
    /* { "append",   builtin_append,   2, { T_ANY, T_LIST } }, */
    /* { "append",   builtin_append,   2, { T_MSTRING, T_MSTRING } }, */
    struct value ret = Value_Empty();
    switch (value_type(args[1])) {
    case T_LIST:
        (void)scope; (void)argc;
        list_append(args[0], value_get_list(&args[1]));
        ret = value_copy(args[1]);
        break;
    case T_MSTRING: ;
        value_set_type(ret, T_MSTRING);
        {
            if (argc > 1) {
                struct mstring *m = value_get_mstring(&args[1]);
                struct mstring *n = value_get_mstring(&args[0]);
                char *extra = (char *)((argc > 1) ? (mstring_cstr(n)) : (NULL));
                mstring_append(m, extra);
                value_set_mstr(ret, m);
            }
        }
        break;
    default: break;
    }
    return ret;
}

static struct value builtin_insert(struct value *args, int argc, struct scope *scope)
{
    (void)scope;
    struct value ret = Value_Empty();
    switch (value_type(args[1])) {
    case T_LIST:
        /* { "insert",   builtin_insert,   2, { T_ANY, T_LIST } }, */
        if (argc > 1) {
            list_insert(args[0], value_get_list(&args[1])); /* copies the value to head of list */
            ret = value_copy(args[1]);
        }
        break;
    case T_MSTRING: ;
        /* { "insert",   builtin_insert,   2, { T_MSTRING, T_MSTRING } }, */
        value_set_type(ret, T_MSTRING);
        {
            struct mstring *m = value_get_mstring(&args[1]);
            struct mstring *extra = ((argc > 1) ? (value_get_mstring(&args[0])) : (NULL));
            mstring_insert(m, mstring_cstr(extra));
            value_set_mstr(ret, m);
        }
        break;
    default: break;
    }
    return ret;
}

/* push the value onto the list */
static struct value builtin_push(struct value *args, int argc, struct scope *scope)
{
    struct value ret = Value_Empty();
    (void)scope;
    if (argc > 1) {
        struct value v = args[0];
        struct list *l = value_get_list(&args[1]);
        switch (value_type(args[1])) {
        case T_LIST: {
            if (argc > 1) {
                list_insert(v, l);
                ret = value_copy(v);
            }
            break;
        }
        default: break;
        }
    }
    return ret;
}

static struct value builtin_pop(struct value *args, int argc, struct scope *scope)
{
    struct value ret;
    (void)scope;
    if (argc < 1 || value_type(args[0]) != T_LIST) {
        ret = Value_Empty();
    } else {
        ret = list_pop(value_get_list(&args[0]));
    }
    return ret;
}

static struct value builtin_join(struct value *args, int argc, struct scope *scope)
{
    (void)scope;
    struct value ret = Value_Empty();
    if (argc < 2 || value_type(args[1]) != T_LIST)
        return ret;

    const char *sep = "";
    if (value_type(args[0]) == T_MSTRING && value_get_mstring(&args[0]))
        sep = mstring_cstr(value_get_mstring(&args[0]));

    struct list *lst = value_get_list(&args[1]);
    struct mstring *out = mstring_new("");
    int first = 1;

    for (struct listnode *n = list_head(lst); n; n = listnode_next(n)) {
        if (!first && sep[0])
            mstring_append(out, sep);
        first = 0;

        struct value nv = listnode_val(n);
        if (value_type(nv) == T_MSTRING && value_get_mstring(&nv))
            mstring_append(out, mstring_cstr(value_get_mstring(&nv)));
        else if (value_type(nv) == T_INT) {
            char buf[32];
            snprintf(buf, sizeof buf, "%d", value_get_int(&nv));
            mstring_append(out, buf);
        }
    }

    value_set_type(ret, T_MSTRING);
    value_set_mstr(ret, out);
    return ret;
}

static struct value builtin_find(struct value *args, int argc, struct scope *scope)
{
    const char *funcname = "find";
    (void)scope;
    struct value ret = Value_Empty();
    if (argc == 2) {
        switch (value_type(args[1])) {
        case T_LIST:
            value_set_int(ret, list_find(args[0], value_get_list(&args[1])));
            diagnose(DIAG_DEBUG, DBG_BUILTIN, 0, 0, "retval: %d: \n", value_get_int(&ret));
            break;
        case T_MSTRING: {
            struct mstring *haystack = value_get_mstring(&args[1]);
            struct mstring *needle = value_get_mstring(&args[0]);
            value_set_int(ret, mstring_find(needle, haystack));
            break;
        }
        default:
            diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "%s: unsupported type\n", funcname);
            break;
        }
    } else
        diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "%s: missing argument\n", funcname);
    return ret;
}

/*
static struct value builtin_X(struct value *args, int argc, struct scope *scope)
{
    const char *funcname = "X";
    (void)scope; (void)argc;
    struct value ret = Value_Empty();
    switch (value_type(args[0])) {
    case T_LIST:
        break;
    case T_MSTRING:
        {
        }
        break;
    default:
        diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0, "%s: unsupported type\n", funcname);
        break;
    }
    return ret;
}
*/
