#include "listaTree.h"
#include "listaCircular.h"
#include "mapa.h"

#include <stdio.h>

listaArvores* criar_listaArvores(unsigned long max){
    return (listaArvores*) criar_listaCircular(max);
}

listaArvores* liberar_listaArvores(listaArvores* lista){
    return (listaArvores*) liberar_listaCircular((listaCircular*) lista, (void*) liberar_mapa);
}

listaArvores* mudar_inicio_listaArvores(listaArvores* lista, long indice){
    return (listaArvores*) mudar_elemento_inicial_listaCircular((listaCircular*)lista, indice);
}

unsigned int pegar_numero_elementos_listaArvores(listaArvores* lista){
    return pegar_numero_elementos_listaCircular((listaCircular*) lista);
}

mapa* pegar_mapa_listaArvores(listaArvores* lista, long indice){
    return (mapa*) pegar_listaCircular((listaCircular*)lista, indice);
}

listaArvores* adicionar_listaArvores(listaArvores* lista, mapa* map, long indice){
    return (listaArvores*)adicionar_listaCircular((listaCircular*)lista, (void*) map, indice);
}

listaArvores* adicionar_ordenadamente_listaArvores(listaArvores* lista, mapa* mapa){
	return (listaArvores*)adicionar_ordenadamente_listaCircular((listaCircular*)lista, (void*)mapa, comparar_pesos_mapa);
}

listaArvores* remover_listaArvores(listaArvores* lista, long indice){
    return (listaArvores*)remover_listaCircular((listaCircular*)lista, indice);
}

void mostrar_listaArvores(listaArvores* lista){
    mostrar_listaCircular((listaCircular*)lista, imprimir_mapa);
}

