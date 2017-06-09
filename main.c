#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
//Funcion para ver si un valor x esta entre los valores de un array v(vector) de tamano s(size)
int estaEn(int x, int* v, int s)
{
	int i;
	for (i = 0; i < s; i++)
		if (x == v[i])
			return 1;
	return 0;
}
int perteneceConFunciones(automataConFunciones* objeto, char* cadena)
{
	int estado = objeto->estadoInicial;
	while (cadena[0] != '\0')
		estado = transicionConFunciones(objeto, estado, cadena[0]), cadena++;
	return estaEn(estado, objeto->estadosFinales, objeto->cantEstadosFinales);
}
int anything(char x)
{
	return 1;
}
int operators(char x)
{
	return (x == '+' || x == '-' || x == '*' || x == '/');
}


int main() {
	char *pruebas[] = { "prueba", "+", "12345", "Prueba", "_asd qwe .s", "-23prueba"};

	printf("Pruebas Identificadores:\n");
	int matrizIdentificadores[18] = {
		2, 1, 2, 2, 2, 2,
		1, 1, 1, 2, 2, 2,
		2, 2, 2, 2, 2, 2 };
	automataConFunciones* identificadores = malloc(sizeof(automataConFunciones));
	initConFunciones(identificadores, (int(*[]) (char x)){ isupper,islower, isdigit, isspace, operators, anything}, matrizIdentificadores, 6, 3, (int[1]){1}, 1, 2);
	int i;
	for(i=0;i<6;i++)
		printf(concat(pruebas[i], " : %d\n"), perteneceConFunciones(identificadores, pruebas[i]));


	printf("Pruebas Operadores:\n");
	int matrizOperadores[12] = {
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0 };
	automataConFunciones* operadores = malloc(sizeof(automataConFunciones));
	initConFunciones(operadores, (int(*[]) (char x)) { isupper, islower, isdigit, isspace, operators, anything }, matrizOperadores, 6, 2, (int[1]) { 1 }, 1, 0);
	for (i = 0; i<6; i++)
		printf(concat(pruebas[i], " : %d\n"), perteneceConFunciones(operadores, pruebas[i]));

	free(operadores);
	free(identificadores);

	getchar();
}
