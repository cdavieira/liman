#ifndef _mapa
#define _mapa

typedef struct map bitmap;

typedef struct mapa mapa;

typedef struct caracter caracter;

unsigned int pegar_ASCII_mapa(mapa* node);

unsigned long pegar_peso_mapa(mapa* node);

bitmap* pegar_bitmap_mapa(mapa* node);

mapa* pegar_sae_mapa(mapa* map);

mapa* pegar_sad_mapa(mapa* map);

mapa* criar_mapa(unsigned int ASCII, unsigned long peso, mapa* sae, mapa* sad);

mapa* liberar_mapa(mapa* map);

mapa* adicionar_rota(mapa* map, mapa* filho, int codigo);

mapa* remover_rota(mapa* map, int codigo);

void imprimir_mapa(mapa* map);

unsigned testar_folha_mapa(mapa* map);

unsigned long contar_nodulos_mapa(mapa* map);

unsigned long calcular_altura_mapa(mapa* map);

unsigned long calcular_altura_node_mapa(mapa* arvore, mapa* node, unsigned long altura_inicial);

unsigned testar_existencia_node_mapa(mapa* arvore, mapa* node);

char* encontrar_rota_node_mapa(mapa* arvore, mapa* node);

mapa* preencher_ASCII_mapa(mapa* map, unsigned ASCII);

mapa* preencher_peso_mapa(mapa* map, unsigned peso);

mapa* preencher_bitmap_mapa(mapa* map, bitmap* bm);

#endif