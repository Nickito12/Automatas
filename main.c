#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "automatas.h"

#ifndef PRUEBA
#ifndef GUI
int main() {
    char *input = malloc(1024);
    char exit[] = "exit";
    int i=0;
    scanf("%s", input);
    while(strcmp(input, exit) != 0) {
        printf("%s es un %s\n", input, (esUn((char*)input) ? esUn((char*)input) : "Nada"));
        scanf("%s", input);
    }
    return 0;

}
#endif
#endif
