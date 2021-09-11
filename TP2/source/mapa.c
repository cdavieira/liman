#include "mapa.h"
#include "conteudo-no.h"
#include "tree.h"

unsigned int pegar_ASCII_mapa(mapa* nodulo){
    return nodulo?pegar_ASCII_caracter((caracter*)pegar_conteudo_arvore((tree*)nodulo)):0;
}

unsigned long pegar_peso_mapa(mapa* nodulo){
    return nodulo?pegar_peso_caracter((caracter*)pegar_conteudo_arvore((tree*)nodulo)):0;
}

mapa* pegar_sae_mapa(mapa* map){
    return (mapa*) pegar_sae_arvore((tree*) map);
}

mapa* pegar_sad_mapa(mapa* map){
    return (mapa*) pegar_sad_arvore((tree*) map);
}

mapa* criar_mapa(unsigned int ASCII, unsigned long peso, mapa* sae, mapa* sad){
    return (mapa*) criar_arvore((void*) criar_caracter(ASCII, peso), (nodulo*)sae, (nodulo*)sad);
}

mapa* liberar_mapa(mapa* map){
    return map?(mapa*) desalocar_arvore((tree*)map, (void*) desalocar_caracter):0;
}

mapa* adicionar_rota(mapa* map, mapa* filho, int codigo){
    return map?(mapa*) adicionar_nodulo((tree*) map, (nodulo*) filho, codigo):0;
}

mapa* remover_rota(mapa* map, int codigo){
    return map?(mapa*) remover_nodulo((tree*)map, codigo):0;
}

void imprimir_mapa(mapa* map){
    if(map) testar_nodulo((nodulo*)map)? imprimir_arvore((tree*)map, (void*) exibir_caracter):imprimir_arvore((tree*)map, (void*) exibir_peso_caracter);
}