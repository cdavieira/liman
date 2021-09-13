#include "compactador.h"
#include "tree.h"
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

mapa* ler_arquivo(char* nome){
    mapa* map = 0;
    if(nome){
        FILE* fpin = fopen(nome, "rb");
        assert(fpin, "Erro ao abrir arquivo para leitura! Terminando programa...");
        unsigned long tabela_ocorrencias[128] = {0, };
        // for(char c=fgetc(fpin);c!=EOF;c=fgetc(fpin)){ printf("%x ", c);}
        for(char c=fgetc(fpin);c!=EOF;c=fgetc(fpin)) tabela_ocorrencias[c]++;
        listaArvores* ls = criar_listaArvores(128);
        for(unsigned i=0;i<128;i++){
            if(tabela_ocorrencias[i]) ls = adicionar_listaArvores(ls, criar_mapa(i, tabela_ocorrencias[i], 0, 0), 0);
        }
        mostrar_listaArvores(ls); //lista os caracteres do que apresenta maior codigo ASCII para o menor
        liberar_listaArvores(ls);
        fclose(fpin);
    }
    return map;
}
