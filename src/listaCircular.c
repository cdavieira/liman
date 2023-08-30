#include "listaCircular.h"
#include <stdio.h>
#include <stdlib.h>

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

static unsigned long acertar_indice(long indice, unsigned long teto);

static celulaCircular* criar_celulaCircular(void* conteudo, celulaCircular* proxima, celulaCircular* anterior);

static celulaCircular* liberar_celulaCircular(celulaCircular* celula, void* (*liberar_conteudo)());

static celulaCircular* pegar_celula_listaCircular(listaCircular* lista, long indice);

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

listaCircular* mudar_elemento_inicial_listaCircular(listaCircular* lista, long indice){
    if(lista){
        unsigned long indice_atualizado = acertar_indice(indice, (*lista).n);
        (*lista).inicio = pegar_celula_listaCircular(lista, indice_atualizado);
    }
    return lista;
}

unsigned int pegar_numero_elementos_listaCircular(listaCircular* lista){
    return lista?(*lista).n:0;
}

void* pegar_listaCircular(listaCircular* lista, long indice){
    if(lista){
        celulaCircular* celula = (*lista).inicio;
        unsigned long indice_atualizado = acertar_indice(indice, (*lista).n);
        for(unsigned long i=0;i<indice_atualizado;i++) if(celula) celula = (*celula).proxima;
        return (*celula).conteudo;
    }
    else return 0;
}

/*   
    sabemos que para o caso de indice positivo: i_elemento = numero_elemento-1
    sabemos ainda que i_elemento + | i_elemento_orientacao_trocada | = n_total
    | i_elemento_orientacao_trocada | = (n_total - numero_elemento) + 1 = n_elemento_orientacao_trocada + 1
*/
listaCircular* adicionar_listaCircular(listaCircular* lista, void* item, long indice){ //lista desordenada
    if(lista){
        if((*lista).n>(*lista).max) return lista;           //não podemos adicionar mais items do que o maximo estabelecido
        celulaCircular *celula = criar_celulaCircular(item, 0, 0), *aux=0;  //criando celula a ser adicionada
        if((*lista).n){                                     //importante: se nao houverem elementos, n=0, o procedimento eh simplificado
            unsigned long indice_atualizado = acertar_indice(indice, indice>0?(*lista).max:(*lista).n);   //aqui o indice é adequado dentro do intervalo [0, (*lista).max] //deve ser assim porque o usuario pode tentar adicionar o elemento numa posicao/indice ainda inexistente na lista
            aux = (indice_atualizado>((*lista).n-1))?(*lista).inicio:pegar_celula_listaCircular(lista, indice_atualizado); //se o indice for maior do que o numero de elementos existentes, entao queremos adicionar o elemento antes do elemento inicial, senao é numa posição generica da lista
            (*celula).proxima = aux;
            (*celula).anterior = (*aux).anterior;
            (*((*aux).anterior)).proxima = celula;
            (*aux).anterior = celula;
            if(!indice_atualizado&&(indice>=0)) (*lista).inicio = celula;       //se o elemento adicionado foi no indice 0, entao devemos modificar o ponto de partida da lista circular
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

listaCircular* adicionar_ordenadamente_listaCircular(listaCircular* lista, void* item, int (*comparar)()){
	if(lista && comparar){
		void* item_comparado=0;
		long indice=-1;
		for(long i=0;i<pegar_numero_elementos_listaCircular(lista);i++){
			item_comparado = pegar_listaCircular(lista, i);
			if(!comparar(item, item_comparado)){		//caso o item não seja maior que o comparado, então item<comparado
				indice = i;
				break;
			}
		}
		lista = adicionar_listaCircular(lista, item, indice);
	}
	return lista;
}

listaCircular* remover_listaCircular(listaCircular* lista, long indice){
    if(lista){
        if((*lista).n){                                     //importante: se nao houverem elementos, n=0, o procedimento eh simplificado
            unsigned long indice_atualizado = acertar_indice(indice, (*lista).n); //aqui o indice é atualizado dentro do intervalo [0, (*lista).n]
            celulaCircular* removido = pegar_celula_listaCircular(lista, indice_atualizado);
            if(removido==(*lista).inicio) (*lista).inicio = (*lista).n!=1?(*removido).proxima:0;  //caso queira-se deletar o elemento inicial ou caso a lista apresente um unico elemento
            (*((*removido).anterior)).proxima = (*removido).proxima;
            (*((*removido).proxima)).anterior = (*removido).anterior;
            free(removido);
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

//remainder nativo bugado
static unsigned long acertar_indice(long indice, unsigned long teto){
    indice = indice<0?indice+1:indice;
    while(indice<0) indice+=teto;
    return teto?(indice%teto):0;
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

static celulaCircular* pegar_celula_listaCircular(listaCircular* lista, long indice){
    celulaCircular* celula_indexada = 0;
    if(lista){
        if((*lista).inicio){
            unsigned long indice_atualizado = acertar_indice(indice, (*lista).n);
            celula_indexada = (*lista).inicio;
            for(unsigned long i=0;i<indice_atualizado;i++) if(celula_indexada) celula_indexada = (*celula_indexada).proxima;
            return celula_indexada;
        }
    }
    return celula_indexada;
}
