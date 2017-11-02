%{
#include <stdio.h>
#include <stdlib.h>
#define YY_DECL int yylex()
#include "bison.tab.h"

int yywrap (void )
{
    return 1;
}
unsigned int linea = 0;
%}
blancos      [ \t\n]+

identificador [A-Z]+
pres (int|string|void|return|if|for|while)
cte [1-9]([0-9]*)
puntuacion (\(|\)|,|;)
operadores (\+|\/|\*|-|<|>|&&|\|\||=|\{|\})

%%

{blancos}    /* skip blanks and tabs */;
{identificador} {yylval.sval = strdup(yytext); return IDENTIFICADOR;}
int {return PR_INT;}
string {return PR_STRING;}
void {return PR_VOID;}
return {return PR_RETURN;}
if {return PR_IF;}
for {return PR_FOR;}
while {return PR_WHILE;}
else {return PR_ELSE;}
{cte} {yylval.ival = atoi(yytext); return CTE;}
\( {return PAREN_IZQ;}
\) {return PAREN_DER;}
\{ {return LLAVE_IZQ;}
\} {return LLAVE_DER;}
\, {return COMA;}
\; {return PTO_COMA;}
\+ {return OP_MAS;}
\/ {return OP_DIV;}
\* {return OP_POR;}
- {return OP_MENOS;}
\< {return OP_MENOR;}
\> {return OP_MAYOR;}
&& {return OP_Y;}
\|\| {return OP_O;}
= {return OP_IGUAL;}


. {yyerror("Caracter desconocido\n");};

%%
