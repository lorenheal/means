#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "meanslist.h"
#include "mstring.h"


struct value Value_Empty(void)  /* hhh */
	{
	struct value ret = { .vtype = T_EMPTY };
	return ret;
	}

struct value Value_Copy(struct value v) /* returns owned copy */
	{
	if (v.vtype == T_WORD && v.mval)
		v.mval->refcount++;
	else if (v.vtype == T_LIST && v.lval)
		list_incref(v.lval);
	return v;
	}

void Value_Free(struct value v) /* hhh */
	{
	/* DEBUG: double-free risk if caller also frees */
	if ((v.vtype == T_WORD) && v.mval)
		mstring_free(v.mval);
	else if ((v.vtype == T_LIST) && v.lval)
		list_decref(v.lval);
	}

void Value_Print(const struct value *v) /* hhh */
	{
	if(v)
		{
		switch(v->vtype)
			{
			case T_NUMBER :
				printf("%ld", v->ival);
				break;
			case T_DECIMAL:
				printf("%g", (double)v->fval);
				break;
			case T_BOOL:
				printf("%s", (v->ival ? "true" : "false"));
				break;
			case T_WORD:
				printf("%s", mstring_cstr(v->mval));
				break;
			case T_LIST:
				print_list(v->lval);
				break;
			default:
				break;
			}
		}
	}

long
Value_AsNumber(struct value v) /* hhh */
	{
	return (v.vtype == T_NUMBER) ? v.ival : 0;
	}

double
Value_AsDecimal(struct value v) /* hhh */
	{
	return (v.vtype == T_DECIMAL) ? v.fval : 0.0;
	}

struct mstring *
Value_AsWord(struct value v) /* hhh */
	{
	return (v.vtype == T_WORD) ? v.mval : NULL;
	}

struct func *
Value_AsFunc(struct value v) /* hhh */
	{
	return (v.vtype == T_FUNC) ? v.fnval : NULL;
	}

struct list *
Value_AsList(struct value v) /* hhh */
	{
	return (v.vtype == T_LIST) ? v.lval : NULL;
	}

const char *
Value_AsCStr(struct value v) /* hhh */
	{
	struct mstring *m = Value_AsWord(v);
	return ((m) ? (mstring_cstr(m)) : (""));
	}

int Value_IsEmpty(struct value v) /* hhh */
	{
	return (T_EMPTY == v.vtype);
	}

enum vtype   Value_Type(struct value v) {return v.vtype; } /* hhh */

struct value Value_Bool(int b) /* hhh */
	{
	struct value v = { .vtype = T_BOOL, .ival = b };
	return v;
	}

struct value Value_Func(struct func *f) /* hhh */
	{
	struct value v = Value_Empty();
	if(f)
		{ 
		v.vtype = T_FUNC; 
		v.fnval = f;
		};
	return v;
	}

struct value Value_Decimal(double x) /* hhh */
	{
	struct value v = { .vtype = T_DECIMAL, .fval = x };
	return v;
	}

struct value Value_Number(long x) /* hhh */
	{
	struct value v = { .vtype = T_NUMBER, .ival = x };
	return v;
	}

struct value Value_Word(const char *s) /* hhh */
	{
	struct value v = Value_Empty();
	if(s)
		{
		v.vtype = T_WORD;
		v.mval = mstring_new(s);
		}
	return v;
	}

struct value Value_List(struct list *lst) /* hhh */
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

int Value_IsNumber(const struct value v) 
	{
	return (Value_Type(v) == T_NUMBER);
	}

int Value_IsDecimal (const struct value v) 
	{
	return (Value_Type(v) == T_DECIMAL);
	}

int Value_IsWord(const struct value v) 
	{
	return (Value_Type(v) == T_WORD);
	}

int Value_IsList(const struct value v) 
	{
	return (Value_Type(v) == T_LIST);
	}


int
Value_IsTrue(struct value v)
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
			return Value_AsNumber(v) != 0;
		default:
			return 0;
		}
	}

/* int Value_IsConstant(const struct value *v) { return v && v->is_const; } */

/* void value_set_constant(struct value *v)
	{ if (v) v->is_const = 1; }
*/

const char *Value_AsCstr(struct value *v) /* hhh */
	{
	if (v)
		{
		return((const char *) mstring_cstr(v->mval));
		}
	return(NULL);
	}

int Value_Equal(struct value a, struct value b) /* hhh */
	{
	if (a.vtype != b.vtype) return 0;
	switch (a.vtype)
		{
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
			return a.lval == b.lval;   /* WARN: identity; no deep compare */
		default:
			return 0;
		}
	}

