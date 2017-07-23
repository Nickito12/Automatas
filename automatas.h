#ifndef AUTOMATA
#define AUTOMATA
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
#endif
int length(const char* cadena);
const char* concat(const char* cadena1, const char* cadena2);
void mostrarMatriz1d(const int* m, int a, int b);
int operators(char x);
int anything(char x);
int estaEn(int x, int* v, int s);
int pertenece(automata* objeto, char* cadena);
unsigned int transicion(automata* objeto, unsigned int estado, char x);
void destroy(automata* objeto);
automata* initConCaracteres(unsigned int* matriz, unsigned int cantEntradas, unsigned int cantEstados, unsigned int *estadosFinales, unsigned int cantEstadosFinales, unsigned int estadoRechazo);
automata* initConFunciones(int(**funciones) (char x), unsigned int* matriz, unsigned int cantEntradas, unsigned int cantEstados, unsigned int *estadosFinales, unsigned int cantEstadosFinales, unsigned int estadoRechazo);
unsigned int numcharConFunciones(char x, automata* objeto);
unsigned int numchar(char x, automata objeto);
char* esUn(const char*);


