#include "listaCircular.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct listaCircular{
    celulaCircular* inicio;
    unsigned long max;
    unsigned long n;
};

struct celulaCircular{
    void* conteudo;
    celulaCircular* proxima;
    celulaCircular* anterior;
};

static unsigned long adequar_indice_listaCircular(listaCircular* lista, unsigned long indice);

static celulaCircular* criar_celulaCircular(void* conteudo, celulaCircular* proxima, celulaCircular* anterior);

static celulaCircular* liberar_celulaCircular(celulaCircular* celula, void* (*liberar_conteudo)());

static celulaCircular* pegar_celula_listaCircular(listaCircular* lista, unsigned long indice);

listaCircular* criar_listaCircular(unsigned long max){
    listaCircular* lista = calloc(1, sizeof(listaCircular));
    (*lista).max = max;
    return lista;
}

listaCircular* liberar_listaCircular(listaCircular* lista, void* (*liberar_conteudo)()){
    if(lista && liberar_conteudo){
        celulaCircular* corrente=(*lista).inicio, *ant=0;
        for(unsigned long i=0;i<(*lista).n;i++){
            ant=corrente;
            corrente=(*corrente).proxima;
            liberar_celulaCircular(ant, liberar_conteudo);
        }
        *lista = (listaCircular){.inicio=0, };
        free(lista);
    }
    return 0;
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

listaCircular* adicionar_listaCircular(listaCircular* lista, void* item, unsigned long indice){
    if(lista){
        if((*lista).n>(*lista).max) return lista;           //não podemos adicionar mais items do que o maximo estabelecido
        celulaCircular *celula = criar_celulaCircular(item, 0, 0), *aux=0;  //criando celula a ser adicionada
        if((*lista).n){                                     //importante: se nao houverem elementos, n=0, o procedimento eh simplificado
            indice = adequar_indice_listaCircular(lista, indice);   //aqui o indice é adequado dentro do intervalo [0, (*lista).max]
            aux = (indice>((*lista).n-1))?(*((*lista).inicio)).anterior:pegar_celula_listaCircular(lista, indice); //se o indice for maior do que o numero de elementos existentes, entao queremos adicionar o elemento antes do elemento inicial, senao é numa posição generica da lista
            (*celula).proxima = aux;
            (*celula).anterior = (*aux).anterior;
            (*((*aux).anterior)).proxima = celula;
            (*aux).anterior = celula;
            if(!indice) (*lista).inicio = celula;       //se o elemento adicionado foi no indice 0, entao devemos modificar o ponto de partida da lista circular
        }
        else{
            (*celula).proxima = celula;
            (*celula).anterior = celula;
            (*lista).inicio = celula;
        }
        (*lista).n++;                               //incrementando o numero de elementos que existem na lista
    }
    return lista;
}

listaCircular* remover_listaCircular(listaCircular* lista, unsigned long indice, void* (*liberar_conteudo)()){
    if(lista){
        if((*lista).n){                                     //importante: se nao houverem elementos, n=0, o procedimento eh simplificado
            indice %= (*lista).n;      //aqui o indice é adequado dentro do intervalo [0, (*lista).n]
            celulaCircular* del = pegar_celula_listaCircular(lista, indice);
            if(del==(*lista).inicio) (*lista).inicio = (*lista).n!=1?(*del).proxima:0;
            (*((*del).anterior)).proxima = (*del).proxima;
            (*((*del).proxima)).anterior = (*del).anterior;
            del = liberar_celulaCircular(del, liberar_conteudo);
            (*lista).n--;                               //decrementando o numero de elementos que existem na lista
        }
    }
    return lista;
}

void mostrar_listaCircular(listaCircular* lista, void (*mostrar_conteudo)()){
    if(lista && mostrar_conteudo){
        if((*lista).inicio){
            mostrar_conteudo((*((*lista).inicio)).conteudo);
            for(celulaCircular* corrente = (*((*lista).inicio)).proxima;corrente!=(*lista).inicio;corrente=(*corrente).proxima){
                printf("  -->  ");
                mostrar_conteudo((*corrente).conteudo);
            }
        }
    }
}


// Funções static

static unsigned long adequar_indice_listaCircular(listaCircular* lista, unsigned long indice){
    if(lista) return (*lista).max?(indice%(*lista).max):0;
    else return 0;
}

static celulaCircular* criar_celulaCircular(void* conteudo, celulaCircular* proxima, celulaCircular* anterior){
    celulaCircular* celula = calloc(1, sizeof(celulaCircular));
    *celula = (celulaCircular) {.conteudo = conteudo, .proxima = proxima, .anterior = anterior};
    return celula;
}

static celulaCircular* liberar_celulaCircular(celulaCircular* celula, void* (*liberar_conteudo)()){
    if(celula){
        if((*celula).conteudo) liberar_conteudo((*celula).conteudo);
        *celula = (celulaCircular){.conteudo=0,};
        free(celula);
    }
    return 0;
}

static celulaCircular* pegar_celula_listaCircular(listaCircular* lista, unsigned long indice){
    celulaCircular* celula_indexada = 0;
    if(lista){
        if((*lista).inicio){
            celula_indexada = (*lista).inicio;
            indice = (*lista).n?indice%(*lista).n:0;
            for(unsigned long i=0;i<indice;i++) if(celula_indexada) celula_indexada = (*celula_indexada).proxima;
            return celula_indexada;
        }
    }
    return celula_indexada;
}