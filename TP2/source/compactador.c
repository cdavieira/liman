#include "compactador.h"
#include "mapa.h"
#include "listaTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void assert(void* p, char* msg);

static void assert(void* p, char* msg){
    if(!p){
        puts(msg);
        exit(1);
    }
}

static unsigned long pegar_indice_maior_numero(unsigned long* v, size_t tam);

static unsigned long pegar_indice_maior_numero(unsigned long* v, size_t tam){
    unsigned long maior = 0, index=0;
    if(v){
        for(unsigned long i=0;i<tam;i++){
            if(v[i]){
                maior=maior?(v[i]>maior?v[i]:maior):v[i];
                index = maior==v[i]?i:index;
            }
        }
    }
    return index;
}

mapa* ler_arquivo(char* nome){
    mapa* map = 0;
    if(nome){
        FILE* fpin = fopen(nome, "rb");
        assert(fpin, "Erro ao abrir arquivo para leitura! Terminando programa...");
        unsigned long tabela_ocorrencias[128] = {0, };      //armazena a frequencia de cada letra no arquivo texto
        // for(char c=fgetc(fpin);c!=EOF;c=fgetc(fpin)){ printf("%x ", c);}
        for(char c=fgetc(fpin);c!=EOF;c=fgetc(fpin)) tabela_ocorrencias[c]++;
        listaArvores* ls = criar_listaArvores(128);
        for(unsigned i=pegar_indice_maior_numero(tabela_ocorrencias, 128);i;i=pegar_indice_maior_numero(tabela_ocorrencias, 128)){
            ls = adicionar_listaArvores(ls, criar_mapa(i, tabela_ocorrencias[i], 0, 0), 0);
            tabela_ocorrencias[i] = 0;
        }
        mostrar_listaArvores(ls); //lista do caracter que apresenta maior codigo ASCII para o maior
        putchar('\n');
        liberar_listaArvores(ls);
        fclose(fpin);
    }
    return map;
}
