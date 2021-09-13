#include "mapa.h"
#include "compactador.h"
#include "listaTree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

/*     mapa* map = criar_mapa(97, 2, 0, 0), *map1 = criar_mapa(98, 1, 0, 0), *map2 = criar_mapa(100, 3, 0, 0);
    listaArvores* lista = criar_listaArvores(3);
    lista = adicionar_listaArvores(lista, map, 0);
    lista = adicionar_listaArvores(lista, map1, 0);
    lista = adicionar_listaArvores(lista, map2, 0);
    mostrar_listaArvores(lista);
    lista = liberar_listaArvores(lista);  */
/*     listaCircular* ls = criar_listaCircular(3);
    int* n = malloc(sizeof(int)), *m=malloc(sizeof(int)), *p=malloc(sizeof(int));
    *n = 1;
    *m = 3;
    *p = 5;
    ls = adicionar_listaCircular(ls, (void*)n, 0);
    ls = adicionar_listaCircular(ls, (void*)m, 2);
    ls = adicionar_listaCircular(ls, (void*)p, 3);
    mostrar_listaCircular(ls);
    ls = remover_listaCircular(ls, 0, (void*)free);
    ls = remover_listaCircular(ls, 0, (void*)free);
    ls = remover_listaCircular(ls, 0, (void*)free);
    liberar_listaCircular(ls, (void*)free); */

    mapa* map_ = ler_arquivo("./respostas/teste.txt");
    // imprimir_mapa(map_);
    liberar_mapa(map_);

    // FILE* file = fopen("./respostas/teste.txt", "r");
/*  
    adicionar_rota(map2, map, 0);
    adicionar_rota(map2, map1, 1);

    imprimir_mapa(map2);

    liberar_mapa(map2);
*/
    // fclose(file);

    return 0;
}

/*  
    FILE* file = fopen("./respostas/teste.bin", "wb");
    char a[] = "CARLOS E AI QUAL VAI SER";
    for(int i=0;i<(sizeof(a)-1);i++){
        fwrite(&a[i], sizeof(char), 1, file);
    }
    fclose(file);
    file = fopen("./respostas/teste.bin", "rb");
    char b[50];
    fread(b, sizeof(char), 10, file);
    puts(b);
*/