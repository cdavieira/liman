#ifndef _compactador
#define _compactador
#include <stdio.h>
/*  
    pensando bem nao sera necessario que a variavel do conteudo do mapa armazene o bitmap de cada caracter eu acho, pelo menos nao agora
*/

typedef struct listaArvores listaArvores;

typedef struct mapa mapa;

typedef struct map bitmap;

unsigned long pegar_indice_maior_numero(unsigned long* v, size_t tam);

//INCLUIR O SEPARADOR (\0) NO ALGORITMO DE HUFFMAN
mapa* algoritmo_Huffman(listaArvores* lc);

//map deve ter passado pela etapa de huffman
//essa funcao tem como intencao gerar bitmaps e preencher os campos de cada no folha do mapa com seu respectivo bitmap, que contem o seu codigo em bits
void gravar_codigos_mapa(mapa* map); //talvez seja um desperdicio ter feito duas funcoes (uma principal e uma auxiliar aqui no backend)

//o segundo parametro dessa funcao serve para preservar o mapa raiz inicial
static void gerar_codigos_mapa_auxiliar(mapa* filho, mapa* pai);

void exportar_mapa_formato_bitmap(mapa* map, FILE* fpout);

void gerar_mapa_formato_bitmap(mapa* map, bitmap* bm); //funcao auxiliar da funcao escrever mapa

void exportar_texto_formato_bitmap(mapa* map, FILE* fpin, FILE* fpout);


#endif