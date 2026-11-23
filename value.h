#ifndef VALUE_H
#define VALUE_H

#include "means.h"
#include <stddef.h>

enum vtype 
	{
	T_UNTYPED,
	T_EMPTY,
	T_NUMBER, 
	T_DECIMAL, 
	T_BOOL,
	T_WORD,
	T_LIST,
	T_FUNC,
	T_ANY=100
	};
struct vtypename { enum vtype vtype; char *name; };
/*
struct vtypename vtypenames[] = 
{ 
	{T_UNTYPED, "Untyped"},
	{T_EMPTY,"Empty"},
	{T_NUMBER, "Number"},
	{T_DECIMAL, "Decimal"},
	{T_BOOL,"Bool"},
	{T_WORD,"Word"},
	{T_LIST,"List"},
	{T_FUNC,"Func"},
	{T_ANY, "Any"}
};
*/

struct list;          /* defined later */
struct func;          /* defined later */
struct mstring;          /* defined later */

struct value
	{
	enum vtype      vtype;
	long            ival;
	double          fval;
	struct mstring *mval;     /* owned C string when type == T_WORD */
	struct list    *lval;     /* owned when type == T_LIST */
	struct func    *fnval;     
	int           initialized; /* Almost always 1; 0 for type but unassigned arguments */
	};

/**/
int  Value_IsInit(struct value v);          /* 1 if the value may be used */
struct value Value_Init(struct value v, int init);   /* uninitialized = 0, initialized = 1 */
struct value Value_Uninit(struct value v);  /* makes v uninitialized  */
struct value Value_Empty(void);
struct value Value_Copy(struct value v);          /* returns an owned copy */
void         Value_Free(struct value v);          /* free owned copy with this */
int         Value_Print(const struct value *v);   /* number of items printed */
/**/
/* the Value_As* functions pull out a type from the Value abstraction */
const char * Value_AsCStr(struct value v);
double       Value_AsDecimal(struct value v);
long         Value_AsNumber(struct value v);
struct func *Value_AsFunc(struct value v);
struct list *Value_AsList(struct value v);
struct mstring *Value_AsWord(struct value v);
/**/
/* Value_Type and Value_Is* just return enum or bool values */
int          Value_Equal(struct value a, struct value b); 
enum vtype   Value_Type(struct value v);        /*  */
int          Value_IsDecimal(struct value v);
int          Value_IsEmpty(struct value v);
int          Value_IsTrue(struct value v);
int          Value_IsList(const struct value v);
int          Value_IsNumber(struct value v);
int          Value_IsWord(struct value v);
/**/
struct value Value_Bool(int b);
struct value Value_Decimal(double n);
struct value Value_Number(long n);
struct value Value_Word(const char *s);
struct value Value_List(struct list *lst);
struct value Value_Func(struct func *f);
/**/


#endif /* VALUE_H */
