#ifndef HELP_INCLUDE
#define HELP_INCLUDE

#include "means.h"
#include <stddef.h>
 
extern unsigned int debug_mask;
extern unsigned int error_count;

enum severity
	{ DIAG_DEBUG, DIAG_NOTE, DIAG_WARNING, DIAG_ERROR, DIAG_FATAL };

#define DEBUG(X) ((X) & debug_mask)

/* the only list you edit */
#define DBG_AREAS(M) \
    M(OK,      'k', "OK") \
    M(AST,      'a', "AST") \
    M(BUILTIN, 'b', "builtins") \
    M(EMIT,     'e', "emit") \
    M(EXEC,     'x', "exec") \
    M(GRAMMAR,  'g', "grammar") \
    M(LIST,     'l', "list") \
    M(GETOPT,   'o', "getopt") \
    M(MSTRING,  'm', "mstring") \
    M(PARAMS,   'p', "params") \
    M(PRINT,    'P', "print") \
    M(STDIO,    'i', "stdio") \
    M(THING,    't', "thing") \
    M(VALUE,  'u', "value") \
    M(VERBOSE,  'v', "verbose") \
    M(USER,  'y', "yuser") \
    M(SYMTAB,   's', "symtab")

struct dbgopt {
    char        letter;
    unsigned    bit;
    const char *help;
};

/* bit positions, in list order */
enum {
#define M(name, ch, help) DBG_##name##_BIT,
    DBG_AREAS(M)
#undef M
    DBG_NBITS
};

/* the constants call sites use */
#define DBG_OK       (0)
#define DBG_AST      (1u << DBG_AST_BIT)
#define DBG_BUILTIN  (1u << DBG_BUILTIN_BIT)
#define DBG_EMIT     (1u << DBG_EMIT_BIT)
#define DBG_EXEC     (1u << DBG_EXEC_BIT)
#define DBG_GRAMMAR  (1u << DBG_GRAMMAR_BIT)
#define DBG_GETOPT   (1u << DBG_GETOPT_BIT)
#define DBG_LIST     (1u << DBG_LIST_BIT)
#define DBG_MSTRING  (1u << DBG_MSTRING_BIT)
#define DBG_PARAMS   (1u << DBG_PARAMS_BIT)
#define DBG_PRINT    (1u << DBG_PRINT_BIT)
#define DBG_STDIO    (1u << DBG_STDIO_BIT)
#define DBG_THING    (1u << DBG_THING_BIT)
#define DBG_VALUE    (1u << DBG_VALUE_BIT)
#define DBG_VERBOSE  (1u << DBG_VERBOSE_BIT)
#define DBG_SYMTAB   (1u << DBG_SYMTAB_BIT)
#define DBG_USER   (1u << DBG_USER_BIT)

/* the table, same list */
static const struct dbgopt dbgopts[] = {
#define M(name, ch, help) { ch, DBG_##name, help },
    DBG_AREAS(M)
#undef M
    { 0, 0, NULL }
};

typedef struct helpopt
	{ 
	const char *flag; 
	const char *arg; 
	const char *func; 
	}
	Option;

void help(char *, char *, char *);
void nhelp(char *, const Option *);
void getopthelp(char *optstring);


void diagnose(enum severity sev, unsigned area, int line, int col, const char *fmt, ...);
unsigned int setdebug(const char *options);



void debug_usage(void);

char *buildOptString(const Option *);

#endif /* HELP_INCLUDE */

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
	diagnose(DIAG_DEBUG, DBG_PARAMS, e->line, 0, "name: %s type %d (%s) init: %d", e->name, v.vtype,
	         vtypenames[v.vtype].name, Value_IsInit(v));
*/
