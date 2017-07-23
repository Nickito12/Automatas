#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "automatas.h"

//Funciones auxiliares para cadenas
int length(const char* cadena)
{
	unsigned int l = 0;
	while (cadena[l] != '\0')
	{
		l++;
	}
	return l;
}
const char* concat(const char* cadena1, const char* cadena2)
{
	char* cadenaConcat = (char*)malloc(length(cadena1) + length(cadena2) + 1);
	unsigned int i = 0;
	unsigned int j = 0;
	while (cadena1[i] != '\0')
	{
		cadenaConcat[i] = cadena1[i];
		i++;
	}
	while (cadena2[j] != '\0')
	{
		cadenaConcat[i + j] = cadena2[j];
		j++;
	}
	cadenaConcat[i + j] = '\0';
	return (const char*)cadenaConcat;
}

//Trabajamos con matrices como arrays de 1 dimension(Para poder trabajar con punteros y tamano variable)
//Y accedemos con fila*filas+columna a cada elemento
void mostrarMatriz1d(const int* m, int a, int b)
{
	int i, j;
	for (i = 0; i < a; i++)
		for (j = 0; j < b; j++)
			printf("%d,, ", m[(i*a) + j]);

	return;
}



unsigned int numchar(char x, automata objeto)
{
    //0='a'
	if (x >= 'a' && x <= 'z')
		return x - 'a';
	if (x >= 'A' && x <= 'Z')
		return x - 'A' + 'z' - 'a'+1;
	if (x >= '0' && x <= '9')
		return x - '0' + 'z' - 'a' + 'Z' - 'A'+2;
	if (x == ' ')
		return 3 + '9' - '0' + 'z' - 'a' + 'Z' - 'A';
    return 4 + '9' - '0' + 'z' - 'a' + 'Z' - 'A';
}
unsigned int numcharConFunciones(char x, automata* objeto)
{
	int i = 0;
	//Evaluamos cada funcion hasta encontrar la columna correcta
	while (i < objeto->cantEntradas && objeto->funciones[i] != NULL && !(objeto->funciones[i])(x))
		i++;
    return i < objeto->cantEntradas ? i : objeto->estadoRechazo;
}
automata* initConFunciones(int(**funciones) (char x), unsigned int* matriz, unsigned int cantEntradas, unsigned int cantEstados, unsigned int *estadosFinales, unsigned int cantEstadosFinales, unsigned int estadoRechazo)
{
    automata* objeto = malloc(sizeof(automata));
	objeto->cantEntradas = cantEntradas;
	objeto->cantEstados = cantEstados;
	objeto->funciones = funciones;
	objeto->estadoInicial = 0;
	objeto->numchar = numcharConFunciones;
	objeto->estadoRechazo = estadoRechazo;
	objeto->estadosFinales = estadosFinales;
	objeto->cantEstadosFinales = cantEstadosFinales;
	objeto->matriz = matriz;
	return objeto;
}

automata* initConCaracteres(unsigned int* matriz, unsigned int cantEntradas, unsigned int cantEstados, unsigned int *estadosFinales, unsigned int cantEstadosFinales, unsigned int estadoRechazo)
{
    automata* objeto = malloc(sizeof(automata));
	objeto->cantEntradas = cantEntradas;
	objeto->cantEstados = cantEstados;
	objeto->funciones = NULL;
	objeto->estadoInicial = 0;
	objeto->numchar = numchar;
	objeto->estadoRechazo = estadoRechazo;
	objeto->estadosFinales = estadosFinales;
	objeto->cantEstadosFinales = cantEstadosFinales;
	objeto->matriz = matriz;
	return objeto;
}
void destroy(automata* objeto)
{
    free(objeto);
    return;
}
unsigned int transicion(automata* objeto, unsigned int estado, char x)
{
	return objeto->matriz[(estado*(objeto->cantEntradas)) + ((objeto->numchar)(x, objeto))];
}
int pertenece(automata* objeto, char* cadena)
{
	int estado = objeto->estadoInicial;
	while (cadena[0] != '\0')
		estado = transicion(objeto, estado, cadena[0]), cadena++;
	return estaEn(estado, objeto->estadosFinales, objeto->cantEstadosFinales);
}
//Funcion para ver si un valor x esta entre los valores de un array v(vector) de tamano s(size)
int estaEn(int x, int* v, int s)
{
	int i;
	for (i = 0; i < s; i++)
		if (x == v[i])
			return 1;
	return 0;
}
int anything(char x)
{
	return 1;
}
int operators(char x)
{
	return (x == '+' || x == '-' || x == '*' || x == '/' || x == '>' || x == '<');
}
int operatorstwo(char x)
{
	return ( x == '|' || x == '&');
}
int puntuacion(char x)
{
	return (x == '(' || x == ')' || x == '{' || x == '}' || x == '[' || x == ']' || x == ',' || x == ';');
}
int digitoNoCero(char x)
{
	return (x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9');
}
int Cero(char x)
{
	return x == '0';
}
int palabraReservada(char x[])
{
    return !strcmp(x, "int") || !strcmp(x, "void") || !strcmp(x, "return") || !strcmp(x, "if") || !strcmp(x, "string") || !strcmp(x, "for") || !strcmp(x, "while");
}
char* esUn(const char* string)
{

    unsigned int(*funcionesIdentificadores[]) (char x) = {isupper, anything};
	unsigned int matrizIdentificadores[18] = {
		1, 2,
		1, 2,
		2, 2};
	automata* identificadores = initConFunciones(funcionesIdentificadores, matrizIdentificadores, 2, 3, (int[1]){1}, 1, 2);

    unsigned int(*funcionesOperadores[]) (char x) = {operators, operatorstwo, anything};
	unsigned int matrizOperadores[12] = {
		1, 2, 3,
		3, 3, 3,
		3, 1, 3,
		3, 3, 3 };
	automata* operadores = initConFunciones(funcionesOperadores, matrizOperadores, 3, 4, (int[1]){1}, 1, 3);

    unsigned int(*funcionesPuntuaciones[]) (char x) = {isupper, islower, isdigit, isspace, puntuacion};
	unsigned int matrizPuntuaciones[15] = {
		2, 2, 2, 2, 1,
		2, 2, 2, 2, 2,
		2, 2, 2, 2, 2};
	automata* puntuaciones = initConFunciones(funcionesPuntuaciones, matrizPuntuaciones, 5, 3, (int[1]){1}, 1, 2);

    unsigned int(*funcionesConstantes[]) (char x) = {digitoNoCero, Cero, anything};
	unsigned int matrizConstantes[9] = {
		1, 2, 2,
		1, 1, 2,
		2, 2, 2};
	automata* constantes = initConFunciones(funcionesConstantes, matrizConstantes, 3, 3, (int[1]){1}, 1, 2);

    automata* automatas[] = {identificadores, operadores, puntuaciones, constantes};
    char nombres[][1024] = {"Identificador\0","Operador\0", "Signo de Puntuacion\0", "Constante\0"};
    int i=0;
    char aux[124]=" ";
    char *ret=NULL;
    char y[] = " y \0";
    char pRes[] = "Palabra Reservada\0";
    for(i=0;i<4;i++){
        if(pertenece(automatas[i], string))
            if(ret) {
                int j=0;
                while((*(ret+j))!='\0'){
                    aux[j]=*(ret+j);
                    ++j;
                }
                aux[j]='\0';
                strcpy(aux,concat(aux, y));
                strcpy(aux,concat(aux, nombres[i]));
                ret=aux;
            } else {
                ret = nombres[i];
            }
    }
    if(palabraReservada(string)){
        if(ret){
            int j=0;
            while((*(ret+j))!='\0'){
                aux[j]=*(ret+j);
                ++j;
            }
            strcpy(aux,concat(aux, y));
            strcpy(aux,concat(aux, pRes));
            ret=aux;
        } else {
            ret = pRes;
        }
    }

    return ret;
}

/* CODIGO VIEJO
//Cada columna de la matriz es cuando la funcion correspondiente devuelve True(!=0)
typedef struct {
	int cantEntradas;
	int cantEstados;
	int estadoInicial;
	int *estadosFinales;
	int cantEstadosFinales;
	int estadoRechazo;
	int* matriz;
	int(**funciones) (char x);
} automataConFunciones;

//Cada columna de la matriz es un caracter
typedef struct {
	int cantEntradas;
	int cantEstados;
	int estadoInicial;
	int *estadosFinales;
	int cantEstadosFinales;
	int *matriz;
	int(*numchar) (char x);
} automataConCaracteres;


//Inicializar automataConCaracteres
void initConCaracteres(automataConCaracteres* objeto, int* matriz, int cantEntradas, int cantEstados, int estadosFinales, int cantEstadosFinales, int(*numchar) (char x))
{
    //Cantidad de columnas
	objeto->cantEntradas = cantEntradas;
	//Cantidad de filas
	objeto->cantEstados = cantEstados;
	//Estado inicial default es 0
	objeto->estadoInicial = 0;
	//Array de estos finales
	objeto->estadosFinales = estadosFinales;
	//Tamano
	objeto->cantEstadosFinales = cantEstadosFinales;
	//Puntero "matriz"
	objeto->matriz = matriz;
	//Funcion que transforma cada caracter a un numero(Para usar ese numero como indice
    //de la matriz)
	objeto->numchar = numchar;
}
int charnum(char x)
{
    //0='a'
	if (x >= 'a' && x <= 'z')
		return x - 'a';
	if (x >= 'A' && x <= 'Z')
		return x - 'A' + 'z' - 'a';
	if (x >= '0' && x <= '9')
		return x - '0' + 'z' - 'a' + 'Z' - 'A';
	if (x == ' ')
		return 1 + '9' - '0' + 'z' - 'a' + 'Z' - 'A';
    return 2 + '9' - '0' + 'z' - 'a' + 'Z' - 'A';
}
int perteneceConFunciones(automata* objeto, char* cadena)
{
	int estado = objeto->estadoInicial;
	while (cadena[0] != '\0')
		estado = transicion(objeto, estado, cadena[0]), cadena++;
	return estaEn(estado, objeto->estadosFinales, objeto->cantEstadosFinales);
}
int transicionConCaracteres(automataConCaracteres* objeto, int estado, char x)
{
	return objeto->matriz[(estado*(objeto->cantEntradas)) + ((objeto->numchar)(x))];
}
int perteneceConCaracteres(automataConCaracteres* objeto, char* cadena)
{
	int estado = objeto->estadoInicial;
	while (cadena[0] != '\0')
		estado = transicionConCaracteres(objeto, estado, cadena[0]), cadena++;
	return estaEn(estado, objeto->estadosFinales, objeto->cantEstadosFinales);
}

void initConFunciones(automataConFunciones* objeto, int(**funciones) (char x), int* matriz, int cantEntradas, int cantEstados, int estadosFinales, int cantEstadosFinales, int estadoRechazo)
{
	objeto->cantEntradas = cantEntradas;
	objeto->cantEstados = cantEstados;
	objeto->funciones = funciones;
	objeto->estadoInicial = 0;
	objeto->estadoRechazo = estadoRechazo;
	objeto->estadosFinales = estadosFinales;
	objeto->cantEstadosFinales = cantEstadosFinales;
	objeto->matriz = matriz;
}
int transicionConFunciones(automataConFunciones* objeto, int estado, char x)
{
	int i = 0;
	//Evaluamos cada funcion hasta encontrar la columna correcta
	while (i < objeto->cantEntradas && !(objeto->funciones[i])(x))
		i++;
	return i < objeto->cantEntradas ? (objeto->matriz[(estado*(objeto->cantEntradas)) + i]) : objeto->estadoRechazo;
}
int perteneceConFunciones(automataConFunciones* objeto, char* cadena)
{
	int estado = objeto->estadoInicial;
	while (cadena[0] != '\0')
		estado = transicionConFunciones(objeto, estado, cadena[0]), cadena++;
	return estaEn(estado, objeto->estadosFinales, objeto->cantEstadosFinales);
}
*/
