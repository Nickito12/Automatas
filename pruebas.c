#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "automatas.h"

#ifdef PRUEBA
int main() {
	char *pruebas[] = { "prueba", "+", "12345", "Prueba", "_asd qwe .s", "-23prueba"};
    int i;

	for(i=0;i<6;i++)
		printf("\"%s\" es un %s\n", pruebas[i], esUn(pruebas[i])? esUn(pruebas[i]):"Nada");

	getchar();
}
#endif
