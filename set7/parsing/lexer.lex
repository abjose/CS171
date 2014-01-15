
%{
/* Include tokens and variables from the parser (NUM, OPEN, CLOSE, yylval) */
#include "parser.tab.hpp"
%}
%option noyywrap

%%

[-+]?(([0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)? {
                               yylval.fval = atof(yytext); 
			       return NUMBER;}

Frame              return FRAME;
translation        return TRANSLAT;
scale              return SCALE;
rotation           return ROT;

#.*$         /* Eat comments   */
[ \t\r\n]+   /* Eat whitespace */


%%
