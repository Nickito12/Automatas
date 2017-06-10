#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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


typedef struct {
	//Tamano matriz
	unsigned int cantEntradas;
	unsigned int cantEstados;
	//Estado inicial(0)
	unsigned int estadoInicial;
	//Array de estados finales
	unsigned int *estadosFinales;
	unsigned int cantEstadosFinales;
	//Estado de rechazo(caracteres indefinidos saltan a este estado)
	unsigned int estadoRechazo;
	//Puntero a array(matriz)
	unsigned int *matriz;
	//Funcion que transforma caracteres en numeros para ser usados de indice
	//En la matriz
	unsigned int(*numchar) (char x, struct automata *objeto);
	//Puntero a array de punteros a funciones de char->entero
	int(**funciones) (char x);
} automata;

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
	return (x == '+' || x == '-' || x == '*' || x == '/');
}


int main() {

    unsigned int(*funciones[]) (char x) = {isupper, islower, isdigit, isspace, operators};

	printf("Pruebas Identificadores:\n");
	unsigned int matrizIdentificadores[18] = {
		2, 1, 2, 2, 2,
		1, 1, 1, 2, 2,
		2, 2, 2, 2, 2 };
	automata* identificadores = initConFunciones(funciones, matrizIdentificadores, 5, 3, (int[1]){1}, 1, 2);

	printf("Pruebas Operadores:\n");
	unsigned int matrizOperadores[12] = {
		0, 0, 0, 0, 1,
		0, 0, 0, 0, 0 };
	automata* operadores = initConFunciones(funciones, matrizOperadores, 5, 2, (int[1]){1}, 1, 0);


	char *pruebas[] = { "prueba", "+", "12345", "Prueba", "_asd qwe .s", "-23prueba"};
    int i;

	for(i=0;i<6;i++)
		printf(concat(pruebas[i], " : %d\n"), pertenece(identificadores, pruebas[i]));

	for (i = 0; i<6; i++)
		printf(concat(pruebas[i], " : %d\n"), pertenece(operadores, pruebas[i]));

	destroy(operadores);
	destroy(identificadores);

	getchar();
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
