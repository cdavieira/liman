#include "mapa.h"
#include "conteudo-mapa.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long tamanho_bits_mapa_auxiliar(mapa* map, mapa* mapa_pai);

unsigned int pegar_ASCII_mapa(mapa* node){
    return node?pegar_ASCII_caracter((caracter*)pegar_conteudo_arvore((tree*)node)):0;
}

unsigned long pegar_peso_mapa(mapa* node){
    return node?pegar_peso_caracter((caracter*)pegar_conteudo_arvore((tree*)node)):0;
}

bitmap* pegar_bitmap_mapa(mapa* node){
    return node?pegar_bmap_caracter((caracter*)pegar_conteudo_arvore((tree*)node)):0;
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
    if(map) imprimir_arvore((tree*)map, exibir_ASCII_caracter, exibir_peso_caracter);
}

unsigned testar_folha_mapa(mapa* map){
    return testar_nodulo_folha((nodulo*) map);
}

unsigned long contar_nodes_mapa(mapa* map){
    return contar_nodulos((tree*) map);
}

unsigned long contar_folhas_mapa(mapa* map){
    return calcular_nodulos_folha((tree*) map);
}

unsigned long calcular_altura_mapa(mapa* map){
    return calcular_altura_arvore((tree*) map);
}

unsigned long calcular_altura_node_mapa(mapa* arvore, mapa* node, unsigned long altura_inicial){
    return calcular_altura_nodulo((tree*) arvore, (nodulo*) node, altura_inicial);
}

unsigned testar_existencia_node_mapa(mapa* arvore, mapa* node){
    return testar_existencia_nodulo((tree*) arvore, (nodulo*) node);
}

char* encontrar_rota_node_mapa(mapa* arvore, mapa* node){
    return encontrar_rota_nodulo((tree*) arvore, (nodulo*) node);
}

mapa* percorrer_mapa(mapa* map, char* rota){
    return (mapa*) ir_ate_nodulo((tree*) map, rota);
}

mapa* preencher_ASCII_mapa(mapa* map, unsigned int ASCII){
    return (mapa*) preencher_arvore((tree*) map, (void*)atualizar_ASCII_caracter((caracter*)pegar_conteudo_arvore((tree*) map), ASCII), 0, 0);
}

mapa* preencher_peso_mapa(mapa* map, unsigned int peso){
    return (mapa*) preencher_arvore((tree*) map, (void*)atualizar_peso_caracter((caracter*)pegar_conteudo_arvore((tree*) map), peso), 0, 0);
}

//lembrete !!!! caracter de terminacao da string nao esta sendo considerado na alocacao
mapa* preencher_bitmap_mapa(mapa* map, bitmap* bm){
    return (mapa*) preencher_arvore((tree*) map, (void*)atualizar_bmap_caracter((caracter*)pegar_conteudo_arvore((tree*) map), bm), 0, 0);
}

mapa* buscar_ASCII_mapa(mapa* map, unsigned* ASCII){
    return (mapa*) buscar_conteudo_arvore((tree*) map, (void*) ASCII, (void*) pegar_endereco_ASCII_mapa, (void*)comparar_ASCII_caracter);
}

unsigned* pegar_endereco_ASCII_mapa(mapa* map){
    return pegar_endereco_ASCII_caracter((caracter*)pegar_conteudo_arvore((tree*)map));
}

unsigned long calcular_tamanho_bits_mapa(mapa* map){
    return map?tamanho_bits_mapa_auxiliar(map, map):0;
}

static unsigned long tamanho_bits_mapa_auxiliar(mapa* map, mapa* mapa_pai){
    if(map){
        if(testar_folha_mapa(map)){
            return calcular_altura_node_mapa(mapa_pai, map, 0)*pegar_peso_mapa(map);
        }
        else return tamanho_bits_mapa_auxiliar(pegar_sae_mapa(map), mapa_pai) + tamanho_bits_mapa_auxiliar(pegar_sad_mapa(map), mapa_pai);
    }
    else return 0;
}
