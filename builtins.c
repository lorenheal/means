#include "builtins.h"
#include "meanslist.h"
#include "mstring.h"
#include "symtab.h"
#include "func.h"
#include "params.h"
#include "value.h"
#include "help.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct param_desc {
    const char *name;
    enum vtype  type;
};


struct builtin_desc
	{
	const char *name;                    /* Means name */
	struct value (*fn)(struct scope *);  /* implementation; pulls args from scope */
	int param_count;                     /* exact arity when is_rest == 0; ignored when is_rest == 1 */
	int is_rest;                         /* 1 = accept any number of args (pure rest) */
	const struct param_desc *params;     /* array of length param_count, or NULL */
	};

/* forward declarations — new signature */
static struct value builtin_print(struct scope *);
static struct value builtin_space(struct scope *scope);
static struct value builtin_newline(struct scope *scope);
static struct value builtin_length(struct scope *);
static struct value builtin_find(struct scope *);
static struct value builtin_empty(struct scope *);
/*
static struct value builtin_roll(struct scope *);
static struct value builtin_first(struct scope *);
static struct value builtin_rest(struct scope *);
static struct value builtin_split(struct scope *);
static struct value builtin_last(struct scope *);
static struct value builtin_append(struct scope *);
static struct value builtin_insert(struct scope *);
static struct value builtin_push(struct scope *);
static struct value builtin_pop(struct scope *);
static struct value builtin_join(struct scope *);
static struct value builtin_exit(struct scope *);
static struct value builtin_read(struct scope *);
*/
/* type vectors for the overloads that need them */
/*
static const enum vtype ty_list[]      = { T_LIST };
static const enum vtype ty_mstring[]   = { T_WORD};
static const enum vtype ty_any_list[]  = { T_ANY, T_LIST };
static const enum vtype ty_mstr_mstr[] = { T_WORD, T_WORD };
static const enum vtype ty_int[]       = { T_NUMBER };
static const enum vtype ty_int_mstr[]  = { T_NUMBER, T_WORD };
static const enum vtype ty_mstr_list[] = { T_WORD, T_LIST };
*/

static const struct builtin_desc builtins[] =
	{
	/* pure-rest / unchecked */
		{ "print",  builtin_print,  0, 1, NULL }, /* returns number of items printed */
		{ "space",  builtin_space,  0, 1, NULL }, /* returns ' ' */
		{ "newline", builtin_newline,  0, 1, NULL }, /* returns '\n' */
		{ "empty", builtin_empty,  0, 1, NULL }, /* returns empty list */
		  /* fixed-arity */
    { "length", builtin_length, 1, 0,
      (struct param_desc[]){ { "item", T_ANY } } },

    { "find",   builtin_find,   2, 0,
      (struct param_desc[]){ { "needle", T_ANY }, { "haystack", T_LIST } } },
    { "find",   builtin_find,   2, 0,
      (struct param_desc[]){ { "needle", T_WORD }, { "haystack", T_WORD } } },

	/*   { "read",   builtin_read,   0, 1, NULL }, */ /* returns 1-item list with line of text */
	/*   { "roll",   builtin_roll,   0, 1, NULL }, */ /* returns its arguments as a (multilevel) list */
	/**/
	/* single-argument overloads */
	/*    { "first",  builtin_first,  1, 0, ty_list }, */
	/*    { "first",  builtin_first,  1, 0, ty_mstring }, */
	/*    { "rest",   builtin_rest,   1, 0, ty_list }, */
	/*    { "rest",   builtin_rest,   1, 0, ty_mstring }, */
	/*    { "split",  builtin_split,  1, 0, ty_list }, */
	/*    { "split",  builtin_split,  1, 0, ty_mstring }, */
	/*    { "split",  builtin_split,  2, 0, \ldots }, */
	/*    { "last",   builtin_last,   1, 0, ty_list }, */
	/*    { "last",   builtin_last,   1, 0, ty_mstring }, */
	/*    { "length", builtin_length, 1, 0, ty_mstring }, */
	/*    { "pop",    builtin_pop,    1, 0, ty_list }, */
	/**/
	    /* two-argument overloads */
	/* example of a typed one:
    { "append",  builtin_append,  2, 0,
      (struct param_desc[]){ {"list", T_LIST}, {"item", T_ANY} } }, */
	/*    { "insert", builtin_insert, 2, 0, ty_any_list }, */
	/*    { "insert", builtin_insert, 2, 0, ty_mstr_mstr }, */
	/*    { "push",   builtin_push,   2, 0, ty_any_list }, */
	/*    { "join",   builtin_join,   2, 0, ty_mstr_list }, */
	/*    { "join", builtin_join, 2, 0, ty_mstr_mstr }, */
	/**/
	/*    { "exit",   builtin_exit,   1, 0, ty_int }, */
	/*    { "exit",   builtin_exit,   2, 0, ty_int_mstr }, */
	/**/
		{ NULL, NULL, 0, 0, NULL }
	};

void register_builtins(struct scope *global)
	{
	const struct builtin_desc *b;
	for (b = builtins; b->name; b++)
		{
		struct func *f = func_create_builtin(b->name, global, b->fn, b->is_rest);
		if (b->param_count > 0 && b->params)
			{
			int i;
			for (i = 0; i < b->param_count; i++)
				{
				struct value v = Value_Empty();
				v.vtype = b->params[i].type;
				func_add_param(f, b->params[i].name, v);
				}
			}
		sym_define_function(b->name, f, global);
		}
	}

int namecheck(const char *funcname, int pos, const char *name, enum vtype type)
	{
	const struct builtin_desc *b;
	for (b = builtins; b->name; b++)
		{
		if (strcmp(b->name, funcname) != 0)
			continue;
		if (pos < 0 || pos >= b->param_count)
			return 0;
		return strcmp(b->params[pos].name, name) == 0
		       && b->params[pos].type == type;
		}
	return 0;
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
			diagnose(DIAG_ERROR,DBG_SYMTAB,0,0,"arguments is not a list.");
			}
		}
	else
		{
		diagnose(DIAG_ERROR,DBG_SYMTAB,0,0,"No arguments list.");
		}
	return(ret);
	}

static struct value
builtin_print(struct scope *scope)
	{
	int count = 0;
	struct list *args = get_arguments(scope);
	while (!list_isempty(args))
		{
		struct value v = list_first(args);
		if (!Value_IsEmpty(v))
			Value_Print(&v);
		Value_Free(v);
		count++;
		args = list_rest(args);
		}
	return Value_Number(count); /* 0 if arguments was empty */
	}
/*
* at the top of each builtin that uses named parameter

if (!namecheck("append", 0, "list", T_LIST) ||
    !namecheck("append", 1, "item", T_ANY))
    diagnose(DIAG_ERROR, DBG_BUILTINS, 0, 0,
             "append: parameter table does not match implementation");
*/


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

static struct value builtin_empty(struct scope *scope)
	{
	(void)scope;
	struct value v = Value_List(list_new());
	return v;
	}

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
	struct value item = sym_value(sym_find_in_scope("item", scope)); /* or whatever name you bound */
	struct value ret  = Value_Empty();
	/**/
	switch (Value_Type(item))
		{
		case T_LIST:
			ret = Value_Number(list_length(Value_AsList(item)));
			break;
		case T_WORD:
			ret = Value_Number(mstring_len(Value_AsWord(item)));
			break;
		default:
			diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0,
			         "length: unsupported type");
			break;
		}
	Value_Free(item);
	return ret;
	}

static struct value
builtin_find(struct scope *scope)
	{
	struct value needle   = sym_value(sym_find_in_scope("needle", scope));
	struct value haystack = sym_value(sym_find_in_scope("haystack", scope));
	struct value ret      = Value_Number(-1);
	/**/
	switch (Value_Type(haystack))
		{
		case T_LIST:
			ret = Value_Number(list_find(needle,Value_AsList(haystack)));
			break;
		case T_WORD:
			ret = Value_Number(mstring_find(Value_AsWord(haystack),
			                             Value_AsWord(needle)));
			break;
		default:
			diagnose(DIAG_ERROR, DBG_BUILTIN, 0, 0,
			         "find: unsupported type");
			break;
		}
	Value_Free(needle);
	Value_Free(haystack);
	return ret;
	}


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
