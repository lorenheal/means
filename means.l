%{
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "means.tab.h"

/*
#define RETURN(tok) do { \
    fprintf(stderr, "tok %d (%s)\n", tok, yytext); \
    return tok; \
} while (0)
*/

#define RETURN(tok) return tok
%}

%option noyywrap
%option yylineno
%option nounput
%option noinput
%option case-insensitive

%%

"the"              { RETURN(THE); }
"then"             { RETURN(THEN); }
"is"               { RETURN(IS); }
"are"              { RETURN(ARE); }
"like"             { RETURN(LIKE); }
"if"               { RETURN(IF); }
"by"               { RETURN(BY); }
"while"            { RETURN(WHILE); }
"else"             { RETURN(ELSE); }
"to"               { RETURN(TO); }
"means"            { RETURN(MEANS); }
"constant"         { RETURN(CONSTANT); }
"empty"            { RETURN(EMPTY); }
"stays" 	       { RETURN(STAYS); }
"do"               { RETURN(DO); }
"done"             { RETURN(DONE); }
"return"           { RETURN(RETURN); }
"and"              { RETURN(AND); }
"plus"             { RETURN(PLUS); }
"minus"            { RETURN(MINUS); }
"times"            { RETURN(TIMES); }
"over"             { RETURN(OVER); }
"greater than"     { RETURN(GREATER_THAN); }
"less than"        { RETURN(LESS_THAN); }
"equal to"         { RETURN(EQUAL_TO); }
"unequal to"       { RETURN(UNEQUAL_TO); }
"at least"         { RETURN(AT_LEAST); }
"at most"          { RETURN(AT_MOST); }
"logical and"      { RETURN(LOGICAL_AND); }
"logical or"       { RETURN(LOGICAL_OR); }
"exclusive or"     { RETURN(EXCLUSIVE_OR); }
"quantity"         { RETURN(QUANTITY); }
"end"              { RETURN(END); }
"true"             { yylval.ival = 1; RETURN( BOOLVAL); }
"false"            { yylval.ival = 0; RETURN( BOOLVAL); }
"."                { RETURN( DOT); }
"#".*              { } 
[0-9]+"."[0-9]+    { yylval.fval = (float)atof(yytext); RETURN( DECIMAL); }
[0-9]+             { yylval.ival = atoi(yytext); RETURN( NUMBER); }
\"[^\"]*\"         { yylval.sval = strdup(yytext+1);
                     yylval.sval[strlen(yytext)-2] = '\0';
					 /* fprintf(stderr, "LEX WORD [%s]\n", yylval.sval);*/
                     RETURN( WORD ); }
[a-z_][a-z0-9_]*   { yylval.sval = strdup(yytext); RETURN( IDENT); }
[ \t\n]+           { }
.                  { fprintf(stderr, "unexpected char: %c\n", yytext[0]); }

%% 
