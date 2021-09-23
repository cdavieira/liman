#include "mapa.h"
#include "compactador.h"
#include "listaTree.h"
#include "tree.h"
#include "bitmap.h"
#include "conteudo-mapa.h"
#include "bitmapPLUS.h"
#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//colocar o nome do arquivo a ser lido no makefile (linha de comando)
int main(int argc, char *argv[]){
    
    mapa* map_ = montar_mapa("./respostas/teste.txt");

    /*
    printf("Mapa do codigo gerado: ");
    imprimir_mapa(map_);
    putchar('\n');
    */
   
    compactar("./respostas/teste.txt", "./respostas/teste.bin", map_);

    descompactar("./respostas/teste.bin", "./respostas/descompactado.txt");

    liberar_mapa(map_);

    return 0;
}