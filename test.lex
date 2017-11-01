%{
int yywrap (void )
{
    return 1;
}
unsigned int linea = 0;
%}

blancos      [ \t]+

identificador [A-Z]+
pres (int|string|void|return|if|for|while)
cte [1-9]([0-9]*)
puntuacion (\(|\)|,|;)
operadores (\+|\/|\*|-|<|>|&&|\|\||=)

%%

{blancos}    /* skip blanks and tabs */
{identificador} {printf("identificador %s en linea %u \n", yytext, linea);}
{pres} {printf("palabra reservada %s en linea %u \n", yytext, linea);}
{cte} {printf("cte %s en linea %u \n", yytext);}
{puntuacion} {printf("caracter de puntuacion %s en linea %u \n", yytext, linea);}
{operadores} {printf("operador %s en linea %u \n", yytext, linea);}
\n {linea++;}
. {printf( "Caracter no reconocido: %s en linea %u \n", yytext, linea);}

%%

int main( int  argc , char**  argv  )
{
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
    else
            yyin = stdin;
	yylex();
}
