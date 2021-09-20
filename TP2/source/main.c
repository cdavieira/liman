#include "mapa.h"
#include "compactador.h"
#include "listaTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "bitmap.h"
#include "conteudo-mapa.h"
#include "bitmapPLUS.h"

unsigned compara(int* a, int*b){
    if(a&&b) return *a==*b?1:0;
    else return 0;
}

//colocar o nome do arquivo a ser lido no makefile (linha de comando)
int main(int argc, char *argv[]){
    mapa* map_ = ler_arquivo("./respostas/teste.txt");

    putchar('\n');    
    imprimir_mapa(map_);
    putchar('\n');    


    // printf("%lu\n", contar_nodes_mapa(map_));

    escrever_arquivo_binario("./respostas/teste.txt", "./respostas/teste.bin", map_);
/*     unsigned int a= 'o';
    mapa* map2 = buscar_ASCII_mapa(map_, &a);
    bitmap* bm = pegar_bitmap_mapa(map2);
    bitmapUnloadContents(bm, stdout); */ //decidi fazer um bitmap gigante que contenha todos os codigos a serem utilizados enfilerados. Isso auxiliara na escrita final do arquivo texto no arquivo binario

    // printf("%lu\n", calcular_tamanho_bits_mapa(map_));

    liberar_mapa(map_);


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

    // FILE* file = fopen("./respostas/teste.txt", "r");
/*  
    adicionar_rota(map2, map, 0);
    adicionar_rota(map2, map1, 1);

    imprimir_mapa(map2);

    liberar_mapa(map2);
*/
    // fclose(file);


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

/*
#define criar_mapa(a) criar_mapa(a,2,0,0)

    listaArvores* ls = criar_listaArvores(5);
    mapa* m0 = criar_mapa('a'), *m1 = criar_mapa('b'), *m2 = criar_mapa('c'), *m3 = criar_mapa('d');
    ls = adicionar_listaArvores(ls, m0, 0);
    ls = adicionar_listaArvores(ls, m1, 1);
    ls = adicionar_listaArvores(ls, m2, 3);
    ls = adicionar_listaArvores(ls, m3, -2);
    mostrar_listaArvores(ls);
    putchar('\n');
    
    ls = remover_listaArvores(ls, 0);
    mostrar_listaArvores(ls);
    
    liberar_listaArvores(ls); */

/*     int* a1 = malloc(sizeof(int)), *b1 = malloc(sizeof(int)), *c1 = malloc(sizeof(int));
    *a1 = 1;
    *b1 = 2;
    *c1 = 2;
 
    tree* a = criar_arvore((void*)a1, 0, 0), *b = criar_arvore((void*)b1, 0, 0), *c = criar_arvore(0, a, b);
    tree* resposta = buscar_conteudo_arvore(c, (void*) c1, pegar_conteudo_arvore, compara);
    if(resposta) printf("%d\n", *(int*)pegar_conteudo_arvore(resposta));
    desalocar_arvore(c, (void*)free);  
*/

/*  
    printf("\n%lu\n", calcular_nodulos_folha((tree*)map_));
    printf("%lu\n", calcular_altura_arvore((tree*)map_));
    mapa* map2 = pegar_sae_mapa(pegar_sae_mapa(pegar_sad_mapa(map_)));
    imprimir_mapa(map2);
    printf("\n%lu\n", calcular_altura_nodulo((tree*)map_, (nodulo*)map2, 0));
    // printf("%u\n", testar_existencia_nodulo((tree*) pegar_sad_mapa(map_), (tree*) map2));
    char* rota = encontrar_rota_nodulo((tree*) map_, (nodulo*) map2);
    puts(rota);
*/

/*  
    mapa* map_ = ler_arquivo("./respostas/teste.txt");
    imprimir_mapa(map_);

    printf("\n%lu\n", calcular_nodulos_folha((tree*)map_));
    printf("%lu\n", calcular_altura_arvore((tree*)map_));
    mapa* map2 = pegar_sae_mapa(pegar_sae_mapa(pegar_sad_mapa(map_)));
    imprimir_mapa(map2);
    printf("\n%lu\n", calcular_altura_nodulo((tree*)map_, (nodulo*)map2, 0));
    // printf("%u\n", testar_existencia_nodulo((tree*) pegar_sad_mapa(map_), (tree*) map2));
    char* rota = encontrar_rota_nodulo((tree*) map_, (nodulo*) map2);
    puts(rota);

    nodulo* node = ir_ate_nodulo((tree*) map_, rota);
    imprimir_mapa((mapa*) node); 
    liberar_mapa(map_);
    free(rota);
*/

/*     unsigned char a[50] = {0, };
    unsigned char b = '2';

    FILE* fpin = fopen("./respostas/binario.bin", "rb");


    //01000011
    //começa do bit menos significativo

    for(size_t j=0;j<5;j++){
        fread(&b, sizeof(unsigned char), 1, fpin);
        printf("%c\n", b);
        for(size_t i=0;i<7;i++){
            printf("COMECO: b = %c, ASCII: %d\n", b, b);
            a[i] = b&0x01;
            printf("MEIO: a[%lu]=%d\n", i, a[i]);
            b=b>>1;
            printf("FIM: b = %c\n", b);
        }
    }


    fclose(fpin);
 */

/*     unsigned int a = 'o';
    mapa* map0 = buscar_ASCII_mapa(map_, &a);
    imprimir_mapa(map0);
    putchar('\n'); */

    /*     
    putchar('\n');
    mapa* map2 = percorrer_mapa(map_, "01");
    bitmap* bm = pegar_bitmap_mapa(map2);

    unsigned char a= '2';

    for(size_t i=0;i<bitmapGetLength(bm);i++){
        a = bitmapGetBit(bm, i);
        printf("%d ", a);
    }
    putchar('\n');

    imprimir_mapa(map2);
    */
