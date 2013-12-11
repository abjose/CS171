
%{
#include <iostream>
/* Include tokens and variables from the parser (NUM, OPEN, CLOSE, yylval) */
#include "parser.tab.hpp"
%}
%option noyywrap

%%

 /*[-+]?(([1-9]+\.[0-9]*)|([1-9]*\.[0-9]+)|([1-9]+))([eE][-+]?[0-9]+)? */
 /*[-+]?(([1-9][0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)? */
 /*[-+]?([0-9]*\.[0-9]+|[0-9]+)  */
[-+]?(([0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)? {
                               yylval.fval = atof(yytext); return FLOAT;
                             }
polyline     return LINETOK;
[ \t\r\n]+   /* Eat whitespace */

%%
