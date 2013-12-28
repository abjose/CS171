
%{
/* Include tokens and variables from the parser (NUM, OPEN, CLOSE, yylval) */
#include "parser.tab.hpp"
%}
%option noyywrap

%%

[-+]?(([0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)? {
                               yylval.fval = atof(yytext); 
			       return NUMBER;}

Separator          return SEPARATOR;
","                return COMMA;
"{"                return LBRACE;
"}"                return RBRACE;
"["                return LBRACKET;
"]"                return RBRACKET;

PerspectiveCamera  return PCAMERA;
position           return POS;
orientation        return ORIENT;
nearDistance       return NDIST;
farDistance        return FDIST;
left               return LEFT;
right              return RIGHT;
top                return TOP;
bottom             return BOTTOM;

Transform          return TRANSFORM;
translation        return TRANSLAT;
scaleFactor        return SFACTOR;
rotation           return ROT;

Coordinate3        return COORD3;
point              return POINT;

Normal             return NORMAL;
vector             return VECTOR;

IndexedFaceSet     return IFACESET;
coordIndex         return COORDINDEX;
normalIndex        return NORMALINDEX;

PointLight         return POINTLIGHT;
location           return LLOC;
color              return LCOLOR;

Material           return MATERIAL;
ambientColor       return AMBCOLOR;
diffuseColor       return DIFFCOLOR;
specularColor      return SPECCOLOR;
shininess          return SHININESS;

#.*$         /* Eat comments   */
[ \t\r\n]+   /* Eat whitespace */


%%
