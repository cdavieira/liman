#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//EXPLICAR FUNCIONAMENTO DUAL DA MAIN NO DOCUMENTO

int main(int argc, char *argv[]){
    if(!strcmp(argv[0], "./Compactador")) compactar(argv[1]);
    else if(!strcmp(argv[0], "./Descompactador")) descompactar(argv[1]);
    return 0;
}