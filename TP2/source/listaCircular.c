#include "listaCircular.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct listaCircular{
    celulaCircular* inicio;
    celulaCircular* fim;
    unsigned long max;
};

struct celulaCircular{
    void* conteudo;
    celulaCircular* proxima;
};

static unsigned long adequar_indice_listaCircular(listaCircular* lista, unsigned long indice);

static unsigned long adequar_indice_listaCircular(listaCircular* lista, unsigned long indice){
    if(lista) return indice%(*lista).max;
    else return 0;
}

static void assert(void* p, char* msg);

static void assert(void* p, char* msg){
    if(!p){
        puts(msg);
        exit(1);
    }
}

listaCircular* criar_listaCircular(unsigned long max){
    listaCircular* lista = calloc(1, sizeof(listaCircular));
    (*lista).max = max;
    return lista;
}

listaCircular* liberar_listaCircular(listaCircular* lista, void* (*liberar_conteudo)()){
    if(lista && liberar_conteudo){
        celulaCircular* corrente = (*lista).inicio, *ant = 0;
        while(corrente){

        }
    }
    return lista;
}

void* pegar_listaCircular(listaCircular* lista, unsigned long indice){
    if(lista){
        celulaCircular* celula = (*lista).inicio;
        indice = adequar_indice_listaCircular(lista, indice);
        for(unsigned long i=0;i<indice;i++) if(celula) celula = (*celula).proxima;
        return (*celula).conteudo;
    }
    else return 0;
}

listaCircular* adicionar_listaCircular(listaCircular* lista, unsigned long indice){
    if(lista){
        celulaCircular* celula = (*lista).inicio;
        indice = adequar_indice_listaCircular(lista, indice);
        for(unsigned long i=0;i<indice;i++) if(celula) celula = (*celula).proxima;
        return celula;
    }
    return lista;
}

listaCircular* remover_listaCircular(listaCircular* lista, unsigned long indice);


celulaCircular* criar_celulaCircular(void* conteudo, celulaCircular* proxima);

celulaCircular* liberar_celulaCircular(celulaCircular* celula, void* (*liberar_conteudo)());