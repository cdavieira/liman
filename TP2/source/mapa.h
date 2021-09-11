#ifndef _mapa
#define _mapa

typedef struct mapa mapa;

typedef struct caracter caracter;

unsigned int pegar_ASCII_mapa(mapa* nodulo);

unsigned long pegar_peso_mapa(mapa* nodulo);

mapa* pegar_sae_mapa(mapa* map);

mapa* pegar_sad_mapa(mapa* map);

mapa* criar_mapa(unsigned int ASCII, unsigned long peso, mapa* sae, mapa* sad);

mapa* liberar_mapa(mapa* map);

mapa* adicionar_rota(mapa* map, mapa* filho, int codigo);

mapa* remover_rota(mapa* map, int codigo);

void imprimir_mapa(mapa* map);

void imprimir_galho_mapa(mapa* map);

void imprimir_folha_mapa(mapa* map);

#endif