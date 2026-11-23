#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "meanslist.h"
#include "mstring.h"


struct value Value_Empty(void) /* def */
	{
	struct value ret = { .vtype = T_EMPTY, .initialized = 1 };
	return ret;
	}

int
Value_IsInit(struct value v) /* def */
{
    return v.initialized;
}

struct value
Value_Init(struct value v, int init) /* def */
{
    v.initialized = init;
    return v;
}

struct value Value_Copy(struct value v) /* def *//* returns owned copy */
	{
	if (v.vtype == T_WORD && v.mval)
		v.mval->refcount++;
	else if (v.vtype == T_LIST && v.lval)
		list_incref(v.lval);
	return v;
	}

void Value_Free(struct value v) /* def */
	{
	/* DEBUG: double-free risk if caller also frees */
	if ((v.vtype == T_WORD) && v.mval)
		mstring_free(v.mval);
	else if ((v.vtype == T_LIST) && v.lval)
		list_decref(v.lval);
	}

int Value_Print(const struct value *v) /* def */
	{
	int printed = 0;
	if(v)
		{
		switch(v->vtype)
			{
			case T_NUMBER :
				printf("%ld", v->ival);
				printed++;
				break;
			case T_DECIMAL:
				printf("%g", (double)v->fval);
				printed++;
				break;
			case T_BOOL:
				printf("%s", (v->ival ? "true" : "false"));
				printed++;
				break;
			case T_WORD:
				printf("%s", mstring_cstr(v->mval));
				printed += (mstring_len(v->mval) > 0?1:0);
				break;
			case T_LIST:
				printed += print_list(v->lval);
				break;
			default:
				break;
			}
		}
	return(printed);
	}

long
Value_AsNumber(struct value v) /* def */
	{
	return (v.vtype == T_NUMBER) ? v.ival : 0;
	}

double
Value_AsDecimal(struct value v) /* def */
	{
	return (v.vtype == T_DECIMAL) ? v.fval : 0.0;
	}

struct mstring *
Value_AsWord(struct value v) /* def */
	{
	return (v.vtype == T_WORD) ? v.mval : NULL;
	}

struct func *
Value_AsFunc(struct value v) /* def */
	{
	return (v.vtype == T_FUNC) ? v.fnval : NULL;
	}

struct list *
Value_AsList(struct value v) /* def */
	{
	return (v.vtype == T_LIST) ? v.lval : NULL;
	}

const char *
Value_AsCStr(struct value v) /* def */
	{
	struct mstring *m = Value_AsWord(v);
	return ((m) ? (mstring_cstr(m)) : (""));
	}

int Value_IsEmpty(struct value v) /* def */
	{
	return (T_EMPTY == v.vtype);
	}

enum vtype   Value_Type(struct value v) /* def */
	{
	return v.vtype;   
	}

struct value Value_Bool(int b) /* def */
	{
	struct value v = { .vtype = T_BOOL, .ival = b };
	return v;
	}

struct value Value_Func(struct func *f) /* def */
	{
	struct value v = Value_Empty();
	if(f)
		{
		v.vtype = T_FUNC;
		v.fnval = f;
		};
	return v;
	}

struct value Value_Decimal(double x) /* def */
	{
	struct value v = { .vtype = T_DECIMAL, .fval = x };
	return v;
	}

struct value Value_Number(long x) /* def */
	{
	struct value v = { .vtype = T_NUMBER, .ival = x };
	return v;
	}

struct value Value_Word(const char *s) 
	{
	struct value v = Value_Empty();
	if(s)
		{
		v.vtype = T_WORD;
		v.mval = mstring_new(s);
		}
	return v;
	}

struct value Value_List(struct list *lst) /* def */
	{
	struct value v = Value_Empty();
	if (lst)
		{
		v.vtype = T_LIST;
		v.lval  = lst;
		list_incref(lst);
		}
	return v;
	}

int Value_IsNumber(const struct value v) /* def */
	{
	return (Value_Type(v) == T_NUMBER);
	}

int Value_IsDecimal (const struct value v) /* def */
	{
	return (Value_Type(v) == T_DECIMAL);
	}

int Value_IsWord(const struct value v) /* def */
	{
	return (Value_Type(v) == T_WORD);
	}

int Value_IsList(const struct value v) /* def */
	{
	return (Value_Type(v) == T_LIST);
	}


int
Value_IsTrue(struct value v) /* def */
	{
	switch (Value_Type(v))
		{
		case T_NUMBER:
			return Value_AsNumber(v) != 0;
		case T_DECIMAL:
			return Value_AsDecimal(v) != 0.0;
		case T_WORD:
			return mstring_len(Value_AsWord(v)) > 0;
		case T_LIST:
			return list_length(Value_AsList(v)) > 0;
		case T_BOOL:
			return v.ival != 0;
		default:
			return 0;
		}
	}

/* int Value_IsConstant(const struct value *v) { return v && v->is_const; } */ /* def */

/* void value_set_constant(struct value *v) */ /* def */ /*
	{ if (v) v->is_const = 1; }
*/

const char *Value_AsCstr(struct value *v) /* def */
	{
	if (v)
		{
		return((const char *) mstring_cstr(v->mval));
		}
	return(NULL);
	}

int Value_Equal(struct value a, struct value b) /* def */
	{
	if (a.vtype != b.vtype) return 0;
	switch (a.vtype)
		{
		case T_EMPTY: return 1;
		case T_NUMBER:
			return a.ival == b.ival;
		case T_DECIMAL:
			return a.fval == b.fval;
		case T_BOOL:
			return a.ival == b.ival;
		case T_WORD:
			{
			int la = a.mval ? mstring_len(a.mval) : 0;
			int lb = b.mval ? mstring_len(b.mval) : 0;
			if (la != lb) return 0;
			if (la == 0) return 1;
			return memcmp(mstring_cstr(a.mval), mstring_cstr(b.mval), la) == 0;
			}
		case T_LIST:
			return list_equal(a.lval, b.lval);
		default:
			return 0;
		}
	}

