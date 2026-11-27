/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "means.y"

#include "symtab.h"
#include "ast.h"
#include "func.h"
#include "exec.h"
#include "help.h"
#include "means.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
void yyerror(const char *s);
extern int yylex(void);

struct scope *defining_scope;
struct stmt *cur_head;

#line 90 "means.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "means.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_BOOLVAL = 4,                    /* BOOLVAL  */
  YYSYMBOL_DECIMAL = 5,                    /* DECIMAL  */
  YYSYMBOL_IDENT = 6,                      /* IDENT  */
  YYSYMBOL_WORD = 7,                       /* WORD  */
  YYSYMBOL_THE = 8,                        /* THE  */
  YYSYMBOL_THEN = 9,                       /* THEN  */
  YYSYMBOL_ARE = 10,                       /* ARE  */
  YYSYMBOL_IS = 11,                        /* IS  */
  YYSYMBOL_BY = 12,                        /* BY  */
  YYSYMBOL_LIKE = 13,                      /* LIKE  */
  YYSYMBOL_IF = 14,                        /* IF  */
  YYSYMBOL_WHILE = 15,                     /* WHILE  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_TO = 17,                        /* TO  */
  YYSYMBOL_MEANS = 18,                     /* MEANS  */
  YYSYMBOL_DO = 19,                        /* DO  */
  YYSYMBOL_DONE = 20,                      /* DONE  */
  YYSYMBOL_RETURN = 21,                    /* RETURN  */
  YYSYMBOL_AND = 22,                       /* AND  */
  YYSYMBOL_PLUS = 23,                      /* PLUS  */
  YYSYMBOL_MINUS = 24,                     /* MINUS  */
  YYSYMBOL_TIMES = 25,                     /* TIMES  */
  YYSYMBOL_OVER = 26,                      /* OVER  */
  YYSYMBOL_LOGICAL_AND = 27,               /* LOGICAL_AND  */
  YYSYMBOL_LOGICAL_OR = 28,                /* LOGICAL_OR  */
  YYSYMBOL_EXCLUSIVE_OR = 29,              /* EXCLUSIVE_OR  */
  YYSYMBOL_GREATER_THAN = 30,              /* GREATER_THAN  */
  YYSYMBOL_LESS_THAN = 31,                 /* LESS_THAN  */
  YYSYMBOL_EQUAL_TO = 32,                  /* EQUAL_TO  */
  YYSYMBOL_UNEQUAL_TO = 33,                /* UNEQUAL_TO  */
  YYSYMBOL_AT_LEAST = 34,                  /* AT_LEAST  */
  YYSYMBOL_AT_MOST = 35,                   /* AT_MOST  */
  YYSYMBOL_QUANTITY = 36,                  /* QUANTITY  */
  YYSYMBOL_END = 37,                       /* END  */
  YYSYMBOL_CONSTANT = 38,                  /* CONSTANT  */
  YYSYMBOL_STAYS = 39,                     /* STAYS  */
  YYSYMBOL_DOT = 40,                       /* DOT  */
  YYSYMBOL_EMPTY = 41,                     /* EMPTY  */
  YYSYMBOL_YYACCEPT = 42,                  /* $accept  */
  YYSYMBOL_program = 43,                   /* program  */
  YYSYMBOL_opt_statement_list = 44,        /* opt_statement_list  */
  YYSYMBOL_statement_list = 45,            /* statement_list  */
  YYSYMBOL_statement = 46,                 /* statement  */
  YYSYMBOL_const_or_not = 47,              /* const_or_not  */
  YYSYMBOL_is_are = 48,                    /* is_are  */
  YYSYMBOL_assignment = 49,                /* assignment  */
  YYSYMBOL_expr_list = 50,                 /* expr_list  */
  YYSYMBOL_return_stmt = 51,               /* return_stmt  */
  YYSYMBOL_if_stmt = 52,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 53,                /* while_stmt  */
  YYSYMBOL_block = 54,                     /* block  */
  YYSYMBOL_funcdef = 55,                   /* funcdef  */
  YYSYMBOL_56_1 = 56,                      /* @1  */
  YYSYMBOL_param = 57,                     /* param  */
  YYSYMBOL_paramlist = 58,                 /* paramlist  */
  YYSYMBOL_call_stmt = 59,                 /* call_stmt  */
  YYSYMBOL_expr = 60,                      /* expr  */
  YYSYMBOL_primary = 61,                   /* primary  */
  YYSYMBOL_call_expr = 62                  /* call_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  38
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   213

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  63
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  112

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   296


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    64,    64,    69,    70,    74,    75,    81,    82,    83,
      84,    85,    86,    87,    91,    92,    96,    97,   101,   103,
     110,   112,   117,   118,   122,   124,   129,   131,   136,   141,
     140,   168,   174,   184,   185,   186,   190,   192,   194,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   218,   219,   220,   221,   222,
     223,   227,   229,   231
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "BOOLVAL",
  "DECIMAL", "IDENT", "WORD", "THE", "THEN", "ARE", "IS", "BY", "LIKE",
  "IF", "WHILE", "ELSE", "TO", "MEANS", "DO", "DONE", "RETURN", "AND",
  "PLUS", "MINUS", "TIMES", "OVER", "LOGICAL_AND", "LOGICAL_OR",
  "EXCLUSIVE_OR", "GREATER_THAN", "LESS_THAN", "EQUAL_TO", "UNEQUAL_TO",
  "AT_LEAST", "AT_MOST", "QUANTITY", "END", "CONSTANT", "STAYS", "DOT",
  "EMPTY", "$accept", "program", "opt_statement_list", "statement_list",
  "statement", "const_or_not", "is_are", "assignment", "expr_list",
  "return_stmt", "if_stmt", "while_stmt", "block", "funcdef", "@1",
  "param", "paramlist", "call_stmt", "expr", "primary", "call_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-60)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      75,    -1,     2,    -1,    -1,     4,    75,    -1,    36,   -60,
      75,   -60,   -25,   -60,   -60,   -60,    12,   -60,    37,   -60,
     -60,   -60,   -60,    33,    41,    -1,   -60,    56,   152,   -60,
     -60,     7,    -4,    87,    72,    62,    -8,   105,   -60,   -60,
     -60,   -60,   -60,   -60,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   -60,   -60,    46,    75,    -1,   -60,    81,   -60,    15,
     -60,   -60,   -60,    56,   152,    49,   165,   165,    24,    24,
     -60,   -60,   178,   178,   178,    79,    79,    79,    79,    79,
      79,   -60,    -1,    77,   123,     7,    71,    72,   -60,    56,
     152,    75,   -60,    38,   -60,   -60,   -60,    68,   -60,    75,
     -60,   -60
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    36,     0,     0,     0,     0,     3,     0,     0,     2,
       4,     5,     0,     8,     9,    10,     0,    13,     0,    55,
      58,    56,    57,     0,     0,     0,    59,    37,    38,    39,
      40,     0,     0,     0,    33,     0,     0,     0,     1,     6,
       7,    12,    11,    60,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    17,    16,    14,     0,     0,    26,     0,    34,     0,
      28,    23,    22,    63,    62,     0,    21,    20,    41,    42,
      43,    44,    51,    52,    53,    45,    46,    47,    48,    49,
      50,    15,     0,    24,     0,     0,     0,     0,    54,    19,
      18,     0,    27,     0,    29,    35,    25,     0,    31,     0,
      32,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -60,   -60,    89,   -60,   -10,   -60,     3,   -60,    -6,   -60,
     -60,   -60,   -24,   -60,   -60,     0,   -60,   -60,     9,   -59,
     -60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     8,     9,    10,    11,    92,    63,    12,    27,    13,
      14,    15,    16,    17,   109,    68,    69,    18,    28,    29,
      30
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      39,    36,    19,    20,    21,    64,    22,    23,    31,    66,
      34,    24,    32,    33,    46,    40,    37,    61,    62,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    71,    96,    45,    25,    38,    97,    73,    43,
      26,    19,    20,    21,   108,    22,    23,    44,   110,    50,
      51,   107,    41,    74,    93,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     102,    19,    20,    21,    94,    22,    23,    42,    46,    26,
      67,     1,    70,     2,    91,    98,    99,    95,   104,     3,
       4,   106,     5,   101,     6,    35,     7,   105,   103,   111,
       0,   100,    48,    49,    50,    51,     6,     0,     0,    26,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,     0,     0,     0,    65,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,     0,     6,     0,     0,    72,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,     0,    75,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    48,    49,    50,    51,     0,     0,     0,    55,    56,
      57,    58,    59,    60
};

static const yytype_int8 yycheck[] =
{
      10,     7,     3,     4,     5,     9,     7,     8,     6,    33,
       6,    12,     3,     4,    22,    40,     7,    10,    11,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    40,    18,    25,    36,     0,    22,    44,     6,
      41,     3,     4,     5,   103,     7,     8,     6,   107,    25,
      26,    13,    40,    44,    64,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      94,     3,     4,     5,    65,     7,     8,    40,    22,    41,
       8,     6,    20,     8,    38,    36,    92,     6,    17,    14,
      15,   101,    17,    16,    19,     6,    21,    97,    95,   109,
      -1,    92,    23,    24,    25,    26,    19,    -1,    -1,    41,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    39,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    19,    -1,    -1,    40,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    37,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    23,    24,    25,    26,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     8,    14,    15,    17,    19,    21,    43,    44,
      45,    46,    49,    51,    52,    53,    54,    55,    59,     3,
       4,     5,     7,     8,    12,    36,    41,    50,    60,    61,
      62,     6,    60,    60,     6,    44,    50,    60,     0,    46,
      40,    40,    40,     6,     6,    60,    22,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    10,    11,    48,     9,    39,    54,     8,    57,    58,
      20,    40,    40,    50,    60,    37,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    38,    47,    46,    60,     6,    18,    22,    36,    50,
      60,    16,    54,    48,    17,    57,    46,    13,    61,    56,
      61,    46
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    45,    46,    46,    46,
      46,    46,    46,    46,    47,    47,    48,    48,    49,    49,
      50,    50,    51,    51,    52,    52,    53,    53,    54,    56,
      55,    57,    57,    58,    58,    58,    59,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    61,    61,    61,    61,    61,
      61,    62,    62,    62
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     2,     1,     1,
       1,     2,     2,     1,     0,     1,     1,     1,     5,     5,
       3,     3,     3,     3,     4,     6,     3,     5,     3,     0,
       7,     4,     5,     0,     1,     3,     1,     2,     2,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     1,     1,     1,     1,     1,
       2,     2,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: opt_statement_list  */
#line 65 "means.y"
        { cur_head = (yyvsp[0].stmtl); }
#line 1234 "means.tab.c"
    break;

  case 3: /* opt_statement_list: %empty  */
#line 69 "means.y"
                                  { (yyval.stmtl) = NULL; }
#line 1240 "means.tab.c"
    break;

  case 4: /* opt_statement_list: statement_list  */
#line 70 "means.y"
                                  { (yyval.stmtl) = (yyvsp[0].stmtl); }
#line 1246 "means.tab.c"
    break;

  case 5: /* statement_list: statement  */
#line 74 "means.y"
                                  { (yyval.stmtl) = (yyvsp[0].sptr); }
#line 1252 "means.tab.c"
    break;

  case 6: /* statement_list: statement_list statement  */
#line 75 "means.y"
                                  { (yyval.stmtl) = stmt_append_list((yyvsp[-1].stmtl), (yyvsp[0].sptr)); }
#line 1258 "means.tab.c"
    break;

  case 7: /* statement: assignment DOT  */
#line 81 "means.y"
                                  { (yyval.sptr) = (yyvsp[-1].sptr); }
#line 1264 "means.tab.c"
    break;

  case 8: /* statement: return_stmt  */
#line 82 "means.y"
                                  { (yyval.sptr) = (yyvsp[0].sptr); }
#line 1270 "means.tab.c"
    break;

  case 9: /* statement: if_stmt  */
#line 83 "means.y"
                                  { (yyval.sptr) = (yyvsp[0].sptr); }
#line 1276 "means.tab.c"
    break;

  case 10: /* statement: while_stmt  */
#line 84 "means.y"
                                  { (yyval.sptr) = (yyvsp[0].sptr); }
#line 1282 "means.tab.c"
    break;

  case 11: /* statement: call_stmt DOT  */
#line 85 "means.y"
                                     { (yyval.sptr) = (yyvsp[-1].sptr); }
#line 1288 "means.tab.c"
    break;

  case 12: /* statement: block DOT  */
#line 86 "means.y"
                                  { (yyval.sptr) = stmt_block((yyvsp[-1].stmtl)); }
#line 1294 "means.tab.c"
    break;

  case 13: /* statement: funcdef  */
#line 87 "means.y"
                                  { (yyval.sptr) = NULL; }
#line 1300 "means.tab.c"
    break;

  case 14: /* const_or_not: %empty  */
#line 91 "means.y"
                                  { (yyval.ival) = 0; }
#line 1306 "means.tab.c"
    break;

  case 15: /* const_or_not: CONSTANT  */
#line 92 "means.y"
                                  { (yyval.ival) = 1; }
#line 1312 "means.tab.c"
    break;

  case 18: /* assignment: THE IDENT is_are const_or_not expr  */
#line 102 "means.y"
        { (yyval.sptr) = stmt_assign((yyvsp[-3].sval), (yyvsp[0].eptr), (yyvsp[-1].ival)); }
#line 1318 "means.tab.c"
    break;

  case 19: /* assignment: THE IDENT is_are const_or_not expr_list  */
#line 104 "means.y"
        { (yyval.sptr) = stmt_assign((yyvsp[-3].sval), ev_list((yyvsp[0].argl)), (yyvsp[-1].ival)); }
#line 1324 "means.tab.c"
    break;

  case 20: /* expr_list: expr AND expr  */
#line 111 "means.y"
        { (yyval.argl) = arglist_append((yyvsp[0].eptr), arglist_new((yyvsp[-2].eptr))); }
#line 1330 "means.tab.c"
    break;

  case 21: /* expr_list: expr_list AND expr  */
#line 113 "means.y"
        { (yyval.argl) = arglist_append((yyvsp[0].eptr), (yyvsp[-2].argl)); }
#line 1336 "means.tab.c"
    break;

  case 22: /* return_stmt: RETURN expr DOT  */
#line 117 "means.y"
                               { (yyval.sptr) = stmt_return(arglist_new((yyvsp[-1].eptr))); }
#line 1342 "means.tab.c"
    break;

  case 23: /* return_stmt: RETURN expr_list DOT  */
#line 118 "means.y"
                               { (yyval.sptr) = stmt_return((yyvsp[-1].argl)); }
#line 1348 "means.tab.c"
    break;

  case 24: /* if_stmt: IF expr THEN statement  */
#line 123 "means.y"
        { (yyval.sptr) = stmt_if((yyvsp[-2].eptr), (yyvsp[0].sptr), NULL); }
#line 1354 "means.tab.c"
    break;

  case 25: /* if_stmt: IF expr THEN statement ELSE statement  */
#line 125 "means.y"
        { (yyval.sptr) = stmt_if((yyvsp[-4].eptr), (yyvsp[-2].sptr), (yyvsp[0].sptr)); }
#line 1360 "means.tab.c"
    break;

  case 26: /* while_stmt: WHILE expr block  */
#line 130 "means.y"
        { (yyval.sptr) = stmt_while((yyvsp[-1].eptr), (yyvsp[0].stmtl)); }
#line 1366 "means.tab.c"
    break;

  case 27: /* while_stmt: WHILE expr STAYS expr block  */
#line 132 "means.y"
        { (yyval.sptr) = stmt_while(ev_binop(STAYS, (yyvsp[-3].eptr), (yyvsp[-1].eptr)), (yyvsp[0].stmtl)); }
#line 1372 "means.tab.c"
    break;

  case 28: /* block: DO opt_statement_list DONE  */
#line 136 "means.y"
                                  { (yyval.stmtl) = (yyvsp[-1].stmtl); }
#line 1378 "means.tab.c"
    break;

  case 29: /* @1: %empty  */
#line 141 "means.y"
        {
          (yyval.scope) = defining_scope;          /* save */
          defining_scope = scope_new((yyvsp[-3].sval), defining_scope);
        }
#line 1387 "means.tab.c"
    break;

  case 30: /* funcdef: TO IDENT paramlist MEANS TO @1 statement  */
#line 146 "means.y"
        {
          struct scope *lex = defining_scope;
          defining_scope = (yyvsp[-1].scope);          /* restore */
          (yyval.fptr) = func_define((yyvsp[-5].sval), (yyvsp[-4].pinfo), (yyvsp[0].sptr), lex);
        }
#line 1397 "means.tab.c"
    break;

  case 31: /* param: THE IDENT is_are primary  */
#line 169 "means.y"
                {
		diagnose(DIAG_DEBUG, DBG_GRAMMAR, "yp1", yylineno, 0, "%s (default)", (yyvsp[-2].sval));
		(yyval.pinfo) = param_from_decl((yyvsp[-2].sval), (yyvsp[0].eptr), 1, defining_scope);
		free_expr((yyvsp[0].eptr));
		}
#line 1407 "means.tab.c"
    break;

  case 32: /* param: THE IDENT is_are LIKE primary  */
#line 175 "means.y"
                {
		diagnose(DIAG_DEBUG, DBG_GRAMMAR, "yp2", yylineno, 0,
				 "%s (required)", (yyvsp[-3].sval));
		(yyval.pinfo) = param_from_decl((yyvsp[-3].sval), (yyvsp[0].eptr), 0, defining_scope);
		free_expr((yyvsp[0].eptr));
		}
#line 1418 "means.tab.c"
    break;

  case 33: /* paramlist: %empty  */
#line 184 "means.y"
                            { (yyval.pinfo) = NULL; }
#line 1424 "means.tab.c"
    break;

  case 34: /* paramlist: param  */
#line 185 "means.y"
                            { (yyval.pinfo) = (yyvsp[0].pinfo); }
#line 1430 "means.tab.c"
    break;

  case 35: /* paramlist: paramlist AND param  */
#line 186 "means.y"
                            { (yyval.pinfo) = param_link((yyvsp[-2].pinfo), (yyvsp[0].pinfo)); }
#line 1436 "means.tab.c"
    break;

  case 36: /* call_stmt: IDENT  */
#line 191 "means.y"
        { (yyval.sptr) = stmt_call((yyvsp[0].sval), NULL); }
#line 1442 "means.tab.c"
    break;

  case 37: /* call_stmt: IDENT expr_list  */
#line 193 "means.y"
        { (yyval.sptr) = stmt_call((yyvsp[-1].sval), (yyvsp[0].argl)); }
#line 1448 "means.tab.c"
    break;

  case 38: /* call_stmt: IDENT expr  */
#line 195 "means.y"
        { (yyval.sptr) = stmt_call((yyvsp[-1].sval), arglist_new((yyvsp[0].eptr))); }
#line 1454 "means.tab.c"
    break;

  case 41: /* expr: expr PLUS expr  */
#line 201 "means.y"
                                          { (yyval.eptr) = ev_binop(PLUS, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1460 "means.tab.c"
    break;

  case 42: /* expr: expr MINUS expr  */
#line 202 "means.y"
                                          { (yyval.eptr) = ev_binop(MINUS, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1466 "means.tab.c"
    break;

  case 43: /* expr: expr TIMES expr  */
#line 203 "means.y"
                                          { (yyval.eptr) = ev_binop(TIMES, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1472 "means.tab.c"
    break;

  case 44: /* expr: expr OVER expr  */
#line 204 "means.y"
                                          { (yyval.eptr) = ev_binop(OVER, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1478 "means.tab.c"
    break;

  case 45: /* expr: expr GREATER_THAN expr  */
#line 205 "means.y"
                                          { (yyval.eptr) = ev_binop(GREATER_THAN, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1484 "means.tab.c"
    break;

  case 46: /* expr: expr LESS_THAN expr  */
#line 206 "means.y"
                                          { (yyval.eptr) = ev_binop(LESS_THAN, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1490 "means.tab.c"
    break;

  case 47: /* expr: expr EQUAL_TO expr  */
#line 207 "means.y"
                                          { (yyval.eptr) = ev_binop(EQUAL_TO, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1496 "means.tab.c"
    break;

  case 48: /* expr: expr UNEQUAL_TO expr  */
#line 208 "means.y"
                                          { (yyval.eptr) = ev_binop(UNEQUAL_TO, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1502 "means.tab.c"
    break;

  case 49: /* expr: expr AT_LEAST expr  */
#line 209 "means.y"
                                          { (yyval.eptr) = ev_binop(AT_LEAST, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1508 "means.tab.c"
    break;

  case 50: /* expr: expr AT_MOST expr  */
#line 210 "means.y"
                                          { (yyval.eptr) = ev_binop(AT_MOST, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1514 "means.tab.c"
    break;

  case 51: /* expr: expr LOGICAL_AND expr  */
#line 211 "means.y"
                                          { (yyval.eptr) = ev_binop(LOGICAL_AND, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1520 "means.tab.c"
    break;

  case 52: /* expr: expr LOGICAL_OR expr  */
#line 212 "means.y"
                                          { (yyval.eptr) = ev_binop(LOGICAL_OR, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1526 "means.tab.c"
    break;

  case 53: /* expr: expr EXCLUSIVE_OR expr  */
#line 213 "means.y"
                                          { (yyval.eptr) = ev_binop(EXCLUSIVE_OR, (yyvsp[-2].eptr), (yyvsp[0].eptr)); }
#line 1532 "means.tab.c"
    break;

  case 54: /* expr: QUANTITY expr END QUANTITY  */
#line 214 "means.y"
                                          { (yyval.eptr) = (yyvsp[-2].eptr); }
#line 1538 "means.tab.c"
    break;

  case 55: /* primary: NUMBER  */
#line 218 "means.y"
                                          { (yyval.eptr) = ev_number((yyvsp[0].ival)); }
#line 1544 "means.tab.c"
    break;

  case 56: /* primary: DECIMAL  */
#line 219 "means.y"
                                          { (yyval.eptr) = ev_decimal((yyvsp[0].fval)); }
#line 1550 "means.tab.c"
    break;

  case 57: /* primary: WORD  */
#line 220 "means.y"
                                          { (yyval.eptr) = ev_word((yyvsp[0].sval)); }
#line 1556 "means.tab.c"
    break;

  case 58: /* primary: BOOLVAL  */
#line 221 "means.y"
                                          { (yyval.eptr) = ev_bool((yyvsp[0].ival)); }
#line 1562 "means.tab.c"
    break;

  case 59: /* primary: EMPTY  */
#line 222 "means.y"
                                          { (yyval.eptr) = ev_list(NULL); }
#line 1568 "means.tab.c"
    break;

  case 60: /* primary: THE IDENT  */
#line 223 "means.y"
                                              { (yyval.eptr) = ev_var((yyvsp[0].sval)); }
#line 1574 "means.tab.c"
    break;

  case 61: /* call_expr: BY IDENT  */
#line 228 "means.y"
        { (yyval.eptr) = ev_call((yyvsp[0].sval), NULL); }
#line 1580 "means.tab.c"
    break;

  case 62: /* call_expr: BY IDENT expr  */
#line 230 "means.y"
        { (yyval.eptr) = ev_call((yyvsp[-1].sval), arglist_new((yyvsp[0].eptr))); }
#line 1586 "means.tab.c"
    break;

  case 63: /* call_expr: BY IDENT expr_list  */
#line 232 "means.y"
        { (yyval.eptr) = ev_call((yyvsp[-1].sval), (yyvsp[0].argl)); }
#line 1592 "means.tab.c"
    break;


#line 1596 "means.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 235 "means.y"


void yyerror(const char *s)
{
    diagnose(DIAG_ERROR, DBG_GRAMMAR,"yye", yylineno, 0, "%s", s);
}
