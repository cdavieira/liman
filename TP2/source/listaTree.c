#include "listaTree.h"
#include "listaCircular.h"
#include "mapa.h"

listaArvores* criar_listaArvores(unsigned long max){
    return (listaArvores*) criar_listaCircular(max);
}

listaArvores* liberar_listaArvores(listaArvores* lista){
    return (listaArvores*) liberar_listaCircular((listaCircular*) lista, (void*) liberar_mapa);
}

listaArvores* mudar_inicio_listaArvores(listaArvores* lista, long indice){
    return (listaArvores*) mudar_inicio_listaCircular((listaCircular*)lista, indice);
}

mapa* pegar_listaArvores(listaArvores* lista, unsigned long indice){
    return (mapa*) pegar_listaCircular((listaCircular*)lista, indice);
}

listaArvores* adicionar_listaArvores(listaArvores* lista, mapa* map, unsigned long indice){
    return (listaArvores*)adicionar_listaCircular((listaCircular*)lista, (void*) map, indice);
}

listaArvores* remover_listaArvores(listaArvores* lista, unsigned long indice){
    return (listaArvores*)remover_listaCircular((listaCircular*)lista, indice, (void*) liberar_mapa);
}

void mostrar_listaArvores(listaArvores* lista){
    mostrar_listaCircular((listaCircular*)lista, imprimir_mapa);
}

