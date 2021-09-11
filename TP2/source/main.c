#include "mapa.h"
#include "compactador.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    //mapa* map = criar_mapa(97, 2, 0, 0), *map1 = criar_mapa(98, 1, 0, 0), *map2 = criar_mapa(0, 3, 0, 0);


    mapa* map_ = ler_arquivo("./respostas/teste.txt");
    imprimir_mapa(map_);
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