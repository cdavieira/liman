#include "compactador.h"
#include "tree.h"
#include "mapa.h"
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
        mapa *aux1=0, *aux2=map;
        for(unsigned i=0, j=0;i<128;i++){
            aux1 = adicionar_rota(aux2, criar_mapa(i, tabela_ocorrencias[i], 0, 0), right);
            imprimir_mapa(aux1);
            aux2 = pegar_sad_mapa(aux1);
            imprimir_mapa(aux2);
        }
       /*  unsigned int tam = 0;
        for(int i=0;i<128;i++) if(tabela_ocorrencias[i]) tam++;
        if(tam){
            mapa* conjunto[tam];
            for(unsigned i=0, j=0;i<128;i++)
                if(tabela_ocorrencias[i]){
                    conjunto[j] = criar_mapa(i, tabela_ocorrencias[i], 0, 0);
                    j++;
                };
        } */
        /* for(int i=0;i<(sizeof(tabela_ocorrencias)/sizeof(unsigned long));i++){
            if(tabela_ocorrencias[i]) printf("%c: %ld\n", i, tabela_ocorrencias[i]);
        } */
        fclose(fpin);
    }
    return map;
}
