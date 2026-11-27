/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MEANS_TAB_H_INCLUDED
# define YY_YY_MEANS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    BOOLVAL = 259,                 /* BOOLVAL  */
    DECIMAL = 260,                 /* DECIMAL  */
    IDENT = 261,                   /* IDENT  */
    WORD = 262,                    /* WORD  */
    THE = 263,                     /* THE  */
    THEN = 264,                    /* THEN  */
    ARE = 265,                     /* ARE  */
    IS = 266,                      /* IS  */
    BY = 267,                      /* BY  */
    LIKE = 268,                    /* LIKE  */
    IF = 269,                      /* IF  */
    WHILE = 270,                   /* WHILE  */
    ELSE = 271,                    /* ELSE  */
    TO = 272,                      /* TO  */
    MEANS = 273,                   /* MEANS  */
    DO = 274,                      /* DO  */
    DONE = 275,                    /* DONE  */
    RETURN = 276,                  /* RETURN  */
    AND = 277,                     /* AND  */
    PLUS = 278,                    /* PLUS  */
    MINUS = 279,                   /* MINUS  */
    TIMES = 280,                   /* TIMES  */
    OVER = 281,                    /* OVER  */
    LOGICAL_AND = 282,             /* LOGICAL_AND  */
    LOGICAL_OR = 283,              /* LOGICAL_OR  */
    EXCLUSIVE_OR = 284,            /* EXCLUSIVE_OR  */
    GREATER_THAN = 285,            /* GREATER_THAN  */
    LESS_THAN = 286,               /* LESS_THAN  */
    EQUAL_TO = 287,                /* EQUAL_TO  */
    UNEQUAL_TO = 288,              /* UNEQUAL_TO  */
    AT_LEAST = 289,                /* AT_LEAST  */
    AT_MOST = 290,                 /* AT_MOST  */
    QUANTITY = 291,                /* QUANTITY  */
    END = 292,                     /* END  */
    CONSTANT = 293,                /* CONSTANT  */
    STAYS = 294,                   /* STAYS  */
    DOT = 295,                     /* DOT  */
    EMPTY = 296                    /* EMPTY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "means.y"

    int               ival;
    float             fval;
    char             *sval;
    struct expr      *eptr;
    struct stmt      *sptr;
    struct stmt      *stmtl;
    struct argnode   *argl;
    struct param     *pinfo;
    struct func      *fptr;
	struct scope     *scope;
    int               type;

#line 119 "means.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MEANS_TAB_H_INCLUDED  */
