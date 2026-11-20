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
	};

/**/
/**/
const char * Value_AsCStr(struct value v);
double       Value_AsDecimal(struct value v);
enum vtype   Value_Type(struct value v);
int          Value_IsDecimal(struct value v);
int          Value_IsEmpty(struct value v);
int          Value_IsTrue(struct value v);
int value_is_list(const struct value *v);
int          Value_IsList(const struct value v);
int          Value_IsNumber(struct value v);
int          Value_IsWord(struct value v);
long         Value_AsNumber(struct value v);
struct func *Value_AsFunc(struct value v);
struct list *Value_AsList(struct value v);
struct mstring *Value_AsWord(struct value v);
struct value Value_Bool(int b);
struct value Value_Copy(struct value v);
struct value Value_Decimal(double n);
struct value Value_Empty(void);
struct value Value_Func(struct func *f);
struct value Value_List(struct list *lst);
struct value Value_Number(long n);
struct value Value_Word(const char *s);
void         Value_Free(struct value v);
void         Value_Print(const struct value *v);

const char *value_cstr(struct value *v);
double value_get_decimal(const struct value *v); 
int value_equal(struct value a, struct value b); 
int value_get_num(const struct value *v);
struct list *value_get_list(const struct value *v); 
struct mstring *value_get_word(const struct value *v);

#endif /* VALUE_H */
