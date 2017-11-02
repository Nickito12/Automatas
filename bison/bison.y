%{
#include <stdio.h>
#include <stdlib.h>
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	int ival;
	char *sval;
}


%token <ival> CTE
%token <sval> IDENTIFICADOR
%token OP_MAS OP_MENOS OP_POR OP_DIV OP_IGUAL OP_MAYOR OP_MENOR OP_Y OP_O
%token PR_WHILE PR_FOR PR_RETURN PR_IF PR_ELSE
%token PTO_COMA COMA PAREN_IZQ PAREN_DER LLAVE_IZQ LLAVE_DER
%token PR_INT PR_VOID PR_STRING

%%
programa: sentencias

sentencias:	sentencia
	| sentencias sentencia

operador: OP_MAS
	| OP_MENOS
	| OP_POR
	| OP_DIV
	| OP_IGUAL
	| OP_MAYOR
	| OP_MENOR
	| OP_Y
	| OP_O

valor: IDENTIFICADOR
	| CTE

bucle:	for
	| while
	| if

if: PR_IF PAREN_IZQ expresion PAREN_DER sentencia else
	| PR_IF PAREN_IZQ expresion PAREN_DER sentencia

else: PR_ELSE sentencia

for: PR_FOR PAREN_IZQ expresion PTO_COMA expresion PTO_COMA expresion PAREN_DER sentencia

while: PR_WHILE PAREN_IZQ expresion PAREN_DER sentencia
parametros:	valor
	| valor COMA parametros

llamadoF: IDENTIFICADOR PAREN_IZQ parametros PAREN_DER
	| IDENTIFICADOR PAREN_IZQ PAREN_DER

argumentos:	tipo IDENTIFICADOR
	| tipo IDENTIFICADOR COMA argumentos
	| tipo COMA argumentos

tipo: PR_INT
	| PR_STRING
	| PR_VOID
	| tipo OP_POR

ret: PR_RETURN valor
	| PR_RETURN

definicionF: tipo IDENTIFICADOR PAREN_IZQ argumentos PAREN_DER sentencia
	| tipo IDENTIFICADOR PAREN_IZQ PAREN_DER sentencia

expresion:	PAREN_IZQ expresion PAREN_DER
	| valor
	| expresion operador expresion
	| llamadoF
	| definicionF

declaracionF: tipo IDENTIFICADOR PAREN_IZQ argumentos PAREN_DER
	| tipo IDENTIFICADOR PAREN_IZQ PAREN_DER

declaracion: tipo IDENTIFICADOR
	| declaracionF

sentencia:	expresion PTO_COMA
	| ret PTO_COMA
	| bucle PTO_COMA
	| declaracion PTO_COMA
	| LLAVE_IZQ LLAVE_DER PTO_COMA
	| LLAVE_IZQ sentencias LLAVE_DER PTO_COMA
%%

int main(int argc, char** argv) {
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
    else
            yyin = stdin;
	if(!yyin)
		yyin = stdin;

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));

}

void yyerror(const char *s) {
	printf("Parse error!  Message: %s\n", s);
	exit(-1);
}
