#include "circular-linked-list.h"
#include <stdio.h>
#include <stdlib.h> //calloc, free

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

listaCircular* criar_listaCircular(const unsigned long max){
    listaCircular* lista = calloc(1, sizeof(listaCircular));
    (*lista).max = max;
    return lista;
}

listaCircular* liberar_listaCircular(listaCircular* lista, void* (*liberar_conteudo)()){
    if(!lista || !liberar_conteudo){
		return NULL;
	}
	celulaCircular* corrente=(*lista).inicio, *ant=NULL;
	const unsigned long size = (*lista).n;
	for(unsigned long i=0; i<size; i++){
		ant=corrente;
		corrente=(*corrente).proxima;
		liberar_celulaCircular(ant, liberar_conteudo);
	}
	(*lista).inicio = NULL;
	free(lista);
    return NULL;
}

listaCircular* mudar_elemento_inicial_listaCircular(listaCircular* lista, long indice){
	if(!lista){
		return NULL;
	}
	unsigned long indice_atualizado = acertar_indice(indice, (*lista).n);
	(*lista).inicio = pegar_celula_listaCircular(lista, indice_atualizado);
    return lista;
}

unsigned int pegar_numero_elementos_listaCircular(listaCircular* lista){
    return lista?(*lista).n:0;
}

void* pegar_listaCircular(listaCircular* lista, const long indice){
	if(!lista){
		return NULL;
	}
	celulaCircular* celula = (*lista).inicio;
	unsigned long indice_atualizado = acertar_indice(indice, (*lista).n);
	for(unsigned long i=0;i<indice_atualizado; i++){
		if(celula){
			celula = (*celula).proxima;
		}
	}
	return (*celula).conteudo;
}

/* 1. para i_elemento > 0: i_elemento = numero_elemento-1
 * 2. n_total = i_elemento + modulo(i_elemento_orientacao_trocada)
 * 3. modulo(i_elemento_orientacao_trocada) = (n_total - numero_elemento) + 1 = n_elemento_orientacao_trocada + 1 */
listaCircular* adicionar_listaCircular(listaCircular* lista, void* item, long indice){ //lista desordenada
	if(!lista){
		return NULL;
	}

	/* Não podemos adicionar mais items do que o maximo estabelecido */
	if((*lista).n > (*lista).max){
		return lista;
	}

	celulaCircular *celula = criar_celulaCircular(item, 0, 0);
	celulaCircular *aux = NULL;
	if((*lista).n){
		/* aqui o indice é adequado dentro do intervalo [0, (*lista).max] */
		/* deve ser assim porque o usuario pode tentar adicionar o elemento
		 * numa posicao/indice ainda inexistente na lista */
		unsigned long indice_atualizado = acertar_indice(indice, indice>0?(*lista).max:(*lista).n);

		/* se o indice for maior do que o numero de elementos existentes, entao
		 * queremos adicionar o elemento antes do elemento inicial, senao é
		 * numa posição generica da lista */
		if(indice_atualizado > ((*lista).n-1)){
			aux = (*lista).inicio;
		}
		else{
			aux = pegar_celula_listaCircular(lista, indice_atualizado);
		}

		(*celula).proxima = aux;
		(*celula).anterior = (*aux).anterior;
		(*((*aux).anterior)).proxima = celula;
		(*aux).anterior = celula;

		/* se o elemento adicionado foi no indice 0, entao devemos modificar o
		 * ponto de partida da lista circular */
		if(!indice_atualizado && (indice>=0)){
			(*lista).inicio = celula;
		}
	}
	else{
		/* importante: se nao houverem elementos, n=0, o procedimento eh
		 * simplificado */
		(*celula).proxima = celula;
		(*celula).anterior = celula;
		(*lista).inicio = celula;
	}
	(*lista).n++;
    return lista;
}

listaCircular* adicionar_ordenadamente_listaCircular(listaCircular* lista, void* item, int (*comparar)()){
	if(!lista || !comparar){
		return NULL;
	}
	void* item_comparado = NULL;
	long indice = -1;
	for(long i=0; i<pegar_numero_elementos_listaCircular(lista); i++){
		item_comparado = pegar_listaCircular(lista, i);
		// caso o item não seja maior que o comparado, então item < comparado
		if(!comparar(item, item_comparado)){		
			indice = i;
			break;
		}
	}
	lista = adicionar_listaCircular(lista, item, indice);
	return lista;
}

listaCircular* remover_listaCircular(listaCircular* lista, long indice){
	if(!lista){
		return NULL;
	}
	if(!lista->n){
		return NULL;
	}

	// indice atualizado pertence ao intervalo [0, (*lista).n]
	unsigned long indice_atualizado = acertar_indice(indice, (*lista).n); 
	celulaCircular* removido = pegar_celula_listaCircular(lista, indice_atualizado);

	// tratamento da deleção do elemento inicial 
	if(removido == (*lista).inicio){
		// deleção do único elemento da lista
		if((*lista).n == 1){
			(*lista).inicio = NULL;  
		}
		// deleção do primeiro elemento da lista
		else{
			(*lista).inicio = (*removido).proxima;  
		}
	}
	(*((*removido).anterior)).proxima = (*removido).proxima;
	(*((*removido).proxima)).anterior = (*removido).anterior;
	free(removido);
	(*lista).n--;                               

    return lista;
}

void mostrar_listaCircular(listaCircular* lista, void (*mostrar_conteudo)()){
	if(!lista || !mostrar_conteudo){
		return ;
	}

	if(!(*lista).inicio){
		return ;
	}

	mostrar_conteudo((*((*lista).inicio)).conteudo);

	celulaCircular* corrente = (*((*lista).inicio)).proxima;
	while(corrente != (*lista).inicio){
		printf("  -->  ");
		mostrar_conteudo((*corrente).conteudo);
		corrente=(*corrente).proxima;
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
	if(!celula){
		return NULL;
	}
	if((*celula).conteudo){
		liberar_conteudo((*celula).conteudo);
	}
	*celula = (celulaCircular){.conteudo=NULL, };
	free(celula);
    return NULL;
}

static celulaCircular* pegar_celula_listaCircular(listaCircular* lista, long indice){
	if(!lista){
		return NULL;
	}

	if(!(*lista).inicio){
		return NULL;
	}
    celulaCircular* celula_indexada = (*lista).inicio;
	const unsigned long indice_atualizado = acertar_indice(indice, (*lista).n);
	for(unsigned long i=0; i<indice_atualizado; i++){
		if(celula_indexada){
			celula_indexada = (*celula_indexada).proxima;
		}
	}
    return celula_indexada;
}
