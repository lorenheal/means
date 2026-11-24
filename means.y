%{
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
%}

%union {
    int               ival;
    float             fval;
    char             *sval;
    struct expr      *eptr;
    struct stmt      *sptr;
    struct stmt      *stmtl;
    struct argnode   *argl;
    struct param     *pinfo;
    struct func      *fptr;
    int               type;
}

%token <ival>  NUMBER BOOLVAL
%token <fval>  DECIMAL
%token <sval>  IDENT WORD
%token <type>  TYPENAME

%token THE THEN IS LIKE IF WHILE ELSE TO MEANS DO DONE RETURN
%token AND PLUS MINUS TIMES OVER
%token LOGICAL_AND LOGICAL_OR EXCLUSIVE_OR
%token GREATER_THAN LESS_THAN EQUAL_TO UNEQUAL_TO AT_LEAST AT_MOST
%token QUANTITY END CONSTANT STAYS DOT
%token EMPTY

%type <eptr>   expr primary
%type <sptr>   statement assignment return_stmt if_stmt while_stmt call_stmt
%type <stmtl>  statement_list block opt_statement_list
%type <argl>   arglist expr_list
%type <pinfo>  paramlist param
%type <fptr>   funcdef
%type <ival>   const_or_not

%nonassoc THEN
%nonassoc ELSE
%left AND
%left LOGICAL_OR LOGICAL_AND EXCLUSIVE_OR
%left EQUAL_TO UNEQUAL_TO GREATER_THAN LESS_THAN AT_LEAST AT_MOST
%left PLUS MINUS
%left TIMES OVER

%%

program
    : opt_statement_list
        { cur_head = $1; }
    ;

opt_statement_list
    : /* empty */                 { $$ = NULL; }
    | statement_list              { $$ = $1; }
    ;

statement_list
    : statement                   { $$ = $1; }
    | statement_list statement    { $$ = stmt_append_list($1, $2); }
    ;

/* Every form that has a natural terminator carries it.
   Compound forms (if/while/funcdef) do not add an extra DOT. */
statement
    : assignment DOT              { $$ = $1; }
    | return_stmt                 { $$ = $1; }
    | if_stmt                     { $$ = $1; }
    | while_stmt                  { $$ = $1; }
    | call_stmt DOT               { $$ = $1; }
    | block DOT                   { $$ = stmt_block($1); }
    | funcdef                     { $$ = NULL; }   /* body already terminated */
    ;

const_or_not
    : /* empty */                 { $$ = 0; }
    | CONSTANT                    { $$ = 1; }
    ;

assignment
    : THE IDENT IS const_or_not expr
        { $$ = stmt_assign($2, $5, $4); }
    | THE IDENT IS const_or_not expr_list
        { $$ = stmt_assign($2, ev_list($5), $4); }
    ;

expr_list
    : expr AND expr
        { $$ = arglist_append($3, arglist_new($1)); }
    | expr_list AND expr
        { $$ = arglist_append($3, $1); }
    ;

return_stmt
    : RETURN arglist DOT          { $$ = stmt_return($2); }
    ;

if_stmt
    : IF expr THEN statement
        { $$ = stmt_if($2, $4, NULL); }
    | IF expr THEN statement ELSE statement
        { $$ = stmt_if($2, $4, $6); }
    ;

while_stmt
    : WHILE expr block
        { $$ = stmt_while($2, $3); }
    | WHILE expr STAYS expr block
        { $$ = stmt_while(ev_binop(STAYS, $2, $4), $5); }
    ;

block
    : DO opt_statement_list DONE  { $$ = $2; }
    ;

funcdef
    : TO IDENT paramlist MEANS TO statement
        {
          diagnose(DIAG_DEBUG, DBG_GRAMMAR, yylineno, 0,
                   "funcdef reducing: %s", $2);
          $$ = func_define($2, $3, $6, defining_scope);
        }
    ;

arglist
    : expr                 %prec AND   { $$ = arglist_new($1); }
    | arglist AND expr     %prec AND   { $$ = arglist_append($3, $1); }
    ;

param
    : THE IDENT IS primary 
        { $$ = param_from_decl($2, $4, 1, defining_scope); free_expr($4); }
    | THE IDENT IS LIKE primary 
        { $$ = param_from_decl($2, $5, 0, defining_scope); free_expr($5); }
    ;

paramlist
    : /* empty */           { $$ = NULL; }
    | param                 { $$ = $1; }
    | paramlist AND param   { $$ = param_link($1, $3); }
    ;

call_stmt
    : IDENT
        { $$ = stmt_call($1, NULL); }
    | IDENT arglist
        { $$ = stmt_call($1, pack_if_multi($2)); }
    ;

expr
    : primary
    | expr PLUS expr                      { $$ = ev_binop(PLUS, $1, $3); }
    | expr MINUS expr                     { $$ = ev_binop(MINUS, $1, $3); }
    | expr TIMES expr                     { $$ = ev_binop(TIMES, $1, $3); }
    | expr OVER expr                      { $$ = ev_binop(OVER, $1, $3); }
    | expr GREATER_THAN expr              { $$ = ev_binop(GREATER_THAN, $1, $3); }
    | expr LESS_THAN expr                 { $$ = ev_binop(LESS_THAN, $1, $3); }
    | expr EQUAL_TO expr                  { $$ = ev_binop(EQUAL_TO, $1, $3); }
    | expr UNEQUAL_TO expr                { $$ = ev_binop(UNEQUAL_TO, $1, $3); }
    | expr AT_LEAST expr                  { $$ = ev_binop(AT_LEAST, $1, $3); }
    | expr AT_MOST expr                   { $$ = ev_binop(AT_MOST, $1, $3); }
    | expr LOGICAL_AND expr               { $$ = ev_binop(LOGICAL_AND, $1, $3); }
    | expr LOGICAL_OR expr                { $$ = ev_binop(LOGICAL_OR, $1, $3); }
    | expr EXCLUSIVE_OR expr              { $$ = ev_binop(EXCLUSIVE_OR, $1, $3); }
    | QUANTITY expr END QUANTITY          { $$ = $2; }
    | IDENT arglist
        { $$ = ev_call($1, pack_if_multi($2)); }
    | IDENT
        { $$ = ev_call($1, NULL); }
    ;

primary
    : NUMBER                              { $$ = ev_number($1); }
    | DECIMAL                             { $$ = ev_decimal($1); }
    | WORD                                { $$ = ev_word($1); }
    | BOOLVAL                             { $$ = ev_bool($1); }
    | EMPTY                               { $$ = ev_list(NULL); }
    | THE IDENT                           { $$ = ev_var($2); }
    ;

%%

void yyerror(const char *s)
{
    diagnose(DIAG_ERROR, DBG_GRAMMAR, yylineno, 0, "%s", s);
}
