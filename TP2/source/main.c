#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//EXPLICAR FUNCIONAMENTO DUAL DA MAIN NO DOCUMENTO

int main(int argc, char *argv[]){
    if(!strcmp(argv[0], "./Compactador")) compactar(argv[1], argv[2]);
    else if(!strcmp(argv[0], "./Descompactador")) descompactar(argv[1], argv[2]);
    return 0;
}

/*  
    Mudanças feitas no final:
        Tive que adicionar o '/0' no mapa de caracteres
        Tive que mudar o tamanho do vetor de leitura de caracters (de 128 para 256)
*/