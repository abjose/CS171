/* Comments go here */
%{
#include <iostream>
#include <list>
  //#include "data.h"
/* Include tokens and variables from the parser (NUM, OPEN, CLOSE, yylval) */
#include "parser.tab.hpp"
using namespace std;
%}
%option noyywrap

%%

[-+]?([0-9]*\.[0-9]+|[0-9]+)  {
                                yylval.fval = atof(yytext); return FLOAT;
                              }
translation     return TRANSTOK;
rotation        return ROTTOK;
scaleFactor     return SCALETOK;
[ \t\r\n]+   /* Eat whitespace */

%%

/* Extra code goes here */


