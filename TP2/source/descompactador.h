#ifndef _descompactador
#define _descompactador
#include <stdio.h>

typedef struct map bitmap;

typedef struct mapa mapa;

//mapa gravado no arquivo binario deve ter sido gerado pela funcao exportar_mapa_formato_bitmap
bitmap* remontar_mapa_forma_bitmap(FILE* fpin);

mapa* reconstruir_mapa_forma_original(bitmap* bm);

void traduzir_mensagem(FILE* fpin, FILE* fpout, mapa* mapa_caracteres);

void analisar_armazenamento_arquivo(FILE* fpin);

void imprimir_conteudo_binario(FILE* fpin);

#endif