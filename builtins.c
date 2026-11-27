#include "builtins.h"
#include "meanslist.h"
#include "mstring.h"
#include "symtab.h"
#include "func.h"
#include "params.h"
#include "value.h"
#include "help.h"
#include "means.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern FILE *yyin;

struct builtin_desc
	{
	const char *name;                    /* Means name */
	struct value (*fn)(struct scope *);  /* implementation; pulls args from scope */
	int param_count;                     /* exact arity when is_rest == 0; ignored when is_rest == 1 */
	int is_rest;                         /* 1 = accept any number of args (pure rest) */
	const enum vtype *types;     /*  NULL */
	};

/* forward declarations — new signature */
static struct value builtin_print(struct scope *);
static struct value builtin_space(struct scope *scope);
static struct value builtin_newline(struct scope *scope);
static struct value builtin_length(struct scope *);
static struct value builtin_find(struct scope *);
static struct value builtin_exit(struct scope *);
static struct value builtin_read(struct scope *);
static struct value builtin_first(struct scope *);
static struct value builtin_rest(struct scope *);
static struct value builtin_last(struct scope *);
static struct value builtin_append(struct scope *);
static struct value builtin_insert(struct scope *);
static struct value builtin_push(struct scope *);
static struct value builtin_pop(struct scope *);
static struct value builtin_split(struct scope *);
/* static struct value builtin_roll(struct scope *);*/
/* static struct value builtin_join(struct scope *); */

/* type vectors for the overloads that need them */

static const enum vtype ty_1_any[]         = { T_ANY };
static const enum vtype ty_1_list[]        = { T_LIST };
/* static const enum vtype ty_1_word[]        = { T_WORD };*/
static const enum vtype ty_1_number[]      = { T_NUMBER };
static const enum vtype ty_2_any_list[]    = { T_ANY, T_LIST };
static const enum vtype ty_2_word_word[]   = { T_WORD, T_WORD };
/* static const enum vtype ty_2_word_list[]   = { T_WORD, T_LIST }; */
static const enum vtype ty_2_number_word[] = { T_NUMBER, T_WORD };

static const struct builtin_desc builtins[] =
	{
		{ "print",   builtin_print,   0, 1, NULL },
		{ "space",   builtin_space,   0, 1, NULL },
		{ "newline", builtin_newline, 0, 1, NULL },
		{ "length",  builtin_length,  1, 0, ty_1_any },
		{ "find",    builtin_find,    2, 0, ty_2_any_list },
		{ "find",    builtin_find,    2, 0, ty_2_word_word },
		{ "exit",    builtin_exit,    0, 1, NULL },               /* exit. */
		{ "exit",    builtin_exit,    1, 0, ty_1_number },        /* exit 1. */
		{ "exit",    builtin_exit,    2, 0, ty_2_number_word },   /* exit 1 and "msg". */
		{ "read", builtin_read, 0, 1, NULL },
		{ "first",  builtin_first,  1, 0, ty_1_any },
		{ "rest",   builtin_rest,   1, 0, ty_1_any },
		{ "split",  builtin_split,  1, 0, ty_1_any  },
		{ "last",   builtin_last,   1, 0, ty_1_any  },
		{ "append", builtin_append, 2, 0, ty_2_any_list },
		{ "append", builtin_append, 2, 0, ty_2_word_word },
		{ "insert", builtin_insert, 2, 0, ty_2_any_list },
		{ "insert", builtin_insert, 2, 0, ty_2_word_word },
		{ "push",   builtin_push,   2, 0, ty_2_any_list },
		{ "pop",    builtin_pop,    1, 0, ty_1_list },
	/*   { "join",   builtin_join,   2, 0, ty_2_any_list }, */
	/*   { "join",   builtin_join,   2, 0, ty_2_word_word}, */
	/*   { "roll",   builtin_roll,   0, 1, NULL },          */
		{ NULL, NULL, 0, 0, NULL }
	};

void register_builtins(struct scope *global) /* def */
	{
	const struct builtin_desc *b;
	for (b = builtins; b->name; b++)
		{
		struct func *f = func_create_builtin(b->name, global, b->fn, b->is_rest);
		if (b->param_count > 0 && b->types)
			{
			int i;
			if (b->param_count > MAX_PARAMS)
				diagnose(DIAG_ERROR, DBG_BUILTIN, "rb", 0, 0,
				         "%s: too many parameters (%d > %d)",
				         b->name, b->param_count, MAX_PARAMS);
			for (i = 0; i < b->param_count; i++)
				{
				char namebuf[32];
				struct value v = Value_EmptyType(b->types[i]);
				snprintf(namebuf, sizeof namebuf, "_%.18s_%d", f->name, i + 1);
				func_add_param(f, namebuf, v);
				}
			}
		sym_define_function(b->name, f, global);
		}
	}

static struct list *get_arguments(struct scope *scope)
	{
	struct list *ret = NULL;
	struct sym *args = sym_find_in_scope("arguments", scope);
	if (args)
		{
		if(Value_IsList(args->val))
			{
			ret = Value_AsList(args->val);
			}
		else
			{
			diagnose(DIAG_ERROR,DBG_SYMTAB,"ga1",0,0,"arguments is not a list.");
			}
		}
	else
		{
		diagnose(DIAG_ERROR,DBG_SYMTAB,"ga2",0,0,"No arguments list.");
		}
	return(ret);
	}

static struct value
builtin_print(struct scope *scope)
	{
	struct list *args = get_arguments(scope);
	struct value spc = Value_Word(" ");
	struct value nl  = Value_Word("\n");
	int nprinted = 0;
	while (!list_isempty(args))
		{
		struct value v = list_first(args);
		int printed = Value_IsEmpty(v) ? 0 : Value_Print(&v);
		Value_Free(v);
		args = list_rest(args);
		if (printed && !list_isempty(args))
			{
			Value_Print(&spc);  
			nprinted += printed;
			}
		}
	Value_Print(&nl);
	return Value_Number(nprinted);
	}

static struct value
builtin_exit(struct scope *scope)
	{
	int code = 0;
	struct value codev = sym_value(sym_find_in_scope("_exit_1", scope));
	struct value msgv  = sym_value(sym_find_in_scope("_exit_2", scope));

	if (!Value_IsEmpty(codev) && Value_IsNumber(codev))
		code = (int)Value_AsNumber(codev);

	if (!Value_IsEmpty(msgv) && Value_IsWord(msgv))
		{
		const char *msg = mstring_cstr(Value_AsWord(msgv));
		diagnose(DIAG_WARNING, DBG_BUILTIN,"be1", 0, 0, "%s", msg);
		}

	Value_Free(codev);
	Value_Free(msgv);

	/* DIAG_FATAL prints (if fmt is non-NULL) then exits with the given code.
	   Passing NULL for fmt makes it silent. */
	diagnose(DIAG_FATAL, (unsigned)code, "be2", 0, 0, NULL);

	return Value_Empty();   /* unreachable */
	}

static struct value
builtin_read(struct scope *scope)
	{
	(void)scope;
	char *line = NULL;
	size_t cap = 0;
	ssize_t n;
	FILE *input = stdin;

	/* was: n = getline(&line, &cap, yyin ? yyin : stdin); */
	n = getline(&line, &cap, input);
	if (n < 0)                      /* EOF or error */
		{
		free(line);
		return Value_Empty();
		}
	if (n > 0 && line[n-1] == '\n')
		line[n-1] = '\0';           /* drop the newline */

	struct value ret = Value_Word(line);
	free(line);
	return ret;
	}
/*
static int
optional_count(struct scope *scope)
	{
	struct list *args = get_arguments(scope);
	if (list_isempty(args))
		return 1;
	struct value v = list_first(args);
	int n = Value_IsNumber(v) ? (int)Value_AsNumber(v) : 1;
	Value_Free(v);
	return n > 0 ? n : 1;
	}
*/

static struct value builtin_space(struct scope *scope)
	{
	(void)scope;
	struct value v = Value_Word(" ");
	return v;
	}

static struct value builtin_newline(struct scope *scope)
	{
	(void)scope;
	struct value v = Value_Word("\n");
	return v;
	}

static struct value
builtin_length(struct scope *scope)
	{
	struct value item = sym_value(sym_find_in_scope("_length_1", scope));
	struct value ret  = Value_Empty();
	/**/
	if(Value_IsEmpty(item))
		{
		diagnose(DIAG_ERROR, DBG_BUILTIN, "ble", 0, 0, "length of what?");
		}
	else
		switch (Value_Type(item))
			{
			case T_LIST:
				ret = Value_Number(list_length(Value_AsList(item)));
				break;
			case T_WORD:
				ret = Value_Number(mstring_len(Value_AsWord(item)));
				break;
			default:
				diagnose(DIAG_ERROR, DBG_BUILTIN,"ble",0, 0, "unsupported vtype (%d)", Value_Type(item));
				break;
			}
	Value_Free(item);
	return ret;
	}

static struct value
builtin_find(struct scope *scope)
	{
	struct value needle   = sym_value(sym_find_in_scope("_find_1", scope));
	struct value haystack = sym_value(sym_find_in_scope("_find_2", scope));
	struct value ret      = Value_Number(-1);
	/**/
	if (Value_IsEmpty(haystack))
		{
		diagnose(DIAG_ERROR, DBG_BUILTIN, "bf", 0, 0, "empty haystack");
		}
	else
		switch (Value_Type(haystack))
			{
			case T_LIST:
				ret = Value_Number(TO_MEANS_INDEX(list_find(needle, Value_AsList(haystack))));
				break;
			case T_WORD:
				if (Value_IsWord(needle))
					ret = Value_Number(TO_MEANS_INDEX(
					                       mstring_find(Value_AsWord(needle), Value_AsWord(haystack))));
				break;
			default:
				diagnose(DIAG_ERROR, DBG_BUILTIN, "bf", 0, 0, "unsupported type");
				break;
			}
	Value_Free(needle);
	Value_Free(haystack);
	return ret;
	}
static struct value builtin_first(struct scope *scope)
	{
	struct value arg = sym_get_value("_first_1", scope);   /* or however the first arg is bound */
	struct value ret = Value_Empty();
	switch (Value_Type(arg))
		{
		case T_LIST:
			if (Value_IsList(arg))
				ret = list_first(Value_AsList(arg));
			break;
		case T_WORD:
			{
			struct mstring *m = Value_AsWord(arg);
			if (m && mstring_len(m) > 0)
				{
				char buf[2] = { *mstring_cstr(m), 0 };
				ret = Value_Word(buf);
				}
			}
		break;
		default:
			break;
		}
	return ret;
	}

static struct value builtin_rest(struct scope *scope)
	{
	struct value arg = sym_get_value("_rest_1", scope);
	struct value ret = Value_Empty();
	switch (Value_Type(arg))
		{
		case T_LIST:
			if (Value_IsList(arg))
				ret = Value_List(list_rest(Value_AsList(arg)));
			break;
		case T_WORD:
			{
			struct mstring *m = Value_AsWord(arg);
			if (m && mstring_len(m) > 1)
				ret = Value_Word((mstring_cstr(m) + 1));
			else
				ret = Value_Word("");
			}
		break;
		default:
			break;
		}
	return ret;
	}

static struct value builtin_split(struct scope *scope)
	{
	struct value arg = sym_get_value("_split_1", scope);
	struct value ret = Value_Empty();
	switch (Value_Type(arg))
		{
		case T_LIST:
			if (Value_IsList(arg))
				ret = Value_List(list_split(Value_AsList(arg)));
			break;
		case T_WORD:
			{
			struct mstring *m = Value_AsWord(arg);
			if (m)
				ret = Value_Word(mstring_cstr(mstring_split(m, 1)));
			}
		break;
		default:
			break;
		}
	return ret;
	}

static struct value builtin_last(struct scope *scope)
	{
	struct value arg = sym_get_value("_last_1", scope);
	struct value ret = Value_Empty();
	switch (Value_Type(arg))
		{
		case T_LIST:
			if (Value_IsList(arg))
				ret = Value_Copy(list_last(Value_AsList(arg)));
			break;
		case T_WORD:
			{
			struct mstring *m = Value_AsWord(arg);
			int len = mstring_len(m);
			if (m && len > 0)
				{
				char buf[2] = { mstring_cstr(m)[len - 1], 0 };
				ret = Value_Word(buf);
				}
			}
		break;
		default:
			diagnose(DIAG_ERROR, DBG_BUILTIN, "bla", 0, 0, "unsupported type\n");
			break;
		}
	return ret;
	}

static struct value builtin_append(struct scope *scope)
	{
	struct value small = sym_get_value("_append_1", scope);
	struct value cont  = sym_get_value("_append_2", scope);
	struct value ret   = Value_Empty();
	/**/
	if (Value_Type(cont) == T_LIST)
		{
		list_append(small, Value_AsList(cont));
		ret = Value_Copy(cont);
		}
	else if (Value_Type(cont) == T_WORD && Value_Type(small) == T_WORD)
		{
		mstring_append(mstring_cstr(Value_AsWord(small)), Value_AsWord(cont));
		ret = Value_Copy(cont);
		}
	return ret;
	}

static struct value builtin_insert(struct scope *scope)
	{
	struct value small = sym_get_value("_insert_1", scope);
	struct value cont  = sym_get_value("_insert_2", scope);
	struct value ret   = Value_Empty();
	/**/
	if (Value_Type(cont) == T_LIST)
		{
		struct list *lst = Value_AsList(cont);
        diagnose(DIAG_DEBUG, DBG_LIST, "bi", 0, 0, "cont type=%d lval=%p ref=%d",
                 Value_Type(cont), (void *)lst, lst ? list_get_refcount(lst) : -1);
		list_insert(small, Value_AsList(cont));
		ret = Value_Copy(cont);
		}
	else if (Value_Type(cont) == T_WORD && Value_Type(small) == T_WORD)
		{
		const char *n = mstring_cstr(Value_AsWord(small));
		mstring_insert(n, Value_AsWord(cont));
		ret = cont;
		}
	return ret;
	}

static struct value builtin_push(struct scope *scope)
	{
	return builtin_insert(scope);
	}

static struct value builtin_pop(struct scope *scope)
	{
	struct value arg = sym_get_value("_pop_1", scope);
	struct value ret = Value_Empty();
	/**/
	if (Value_Type(arg) == T_LIST && Value_IsList(arg))
		{
		ret = list_first(Value_AsList(arg));
		list_split(Value_AsList(arg));
		}
	return ret;
	}

/* static struct value builtin_join(struct scope *scope)
    {
    struct value sep  = sym_get_value("_join_1", scope);
    struct value cont = sym_get_value("_join_2", scope);
    struct value ret  = Value_Empty();

    if (Value_Type(cont) == T_LIST)
        {
        ret = Value_Word(list_join( Value_Type(sep) == T_WORD
                                         ? mstring_cstr(Value_AsWord(sep))
                                         : ""), Value_List(cont));
        }
    else if (Value_Type(sep) == T_WORD && Value_Type(cont) == T_WORD)
        {
        struct mstring *n = mstring_copy(Value_AsWord(sep));
        mstring_append(n, mstring_cstr(Value_AsWord(cont)));
        ret = Value_Word(mstring_cstr(n));
        }
    return ret;
    }
*/
/*
static struct value builtin_roll(struct scope *scope)
    {
    struct list *lst = list_new();
    / * pure rest: walk the scope for every argument bound under numeric names * /
    for (int i = 1; ; i++)
        {
        char name[16];
        snprintf(name, sizeof name, "_roll_%d", i);
        struct sym *s = sym_find_in_scope(name, scope);
        if (!s)
            break;
        list_append(s->val, lst);
        }
    return Value_List(lst);
    }
*/

/*
static struct value
builtin_spaces(struct scope *scope)
	{
	int n = optional_count(scope);
	char *buf = malloc(n + 1);
	memset(buf, ' ', n);
	buf[n] = '\0';
	struct value v = Value_Word(buf);
	free(buf);
	return v;
	}

static struct value
builtin_newlines(struct scope *scope)
	{
	int n = optional_count(scope);
	char *buf = malloc(n + 1);
	memset(buf, '\n', n);
	buf[n] = '\0';
	struct value v = Value_Word(buf);
	free(buf);
	return v;
	}
*/
