#ifndef _compactador
#define _compactador
#include <stdio.h>

typedef struct listaArvores listaArvores;

typedef struct mapa mapa;

typedef struct map bitmap;

mapa* montar_mapa(FILE* fpin);

long pegar_indice_maior_numero(unsigned long* v, size_t tam);

//INCLUIR O SEPARADOR (\0) NO ALGORITMO DE HUFFMAN
/*  algoritmo_Huffman
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* algoritmo_Huffman(listaArvores* lc);

//map deve ter passado pela etapa de huffman
//essa funcao tem como intencao gerar bitmaps e preencher os campos de cada no folha do mapa com seu respectivo bitmap, que contem o seu codigo em bits
/*  gravar_codigos_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void gravar_codigos_mapa(mapa* map); //talvez seja um desperdicio ter feito duas funcoes (uma principal e uma auxiliar aqui no backend)

//o segundo parametro dessa funcao serve para preservar o mapa raiz inicial
/*  gerar_codigos_mapa_auxiliar
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
static void gerar_codigos_mapa_auxiliar(mapa* filho, mapa* pai);

/*  exportar_mapa_formato_bitmap
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void exportar_mapa_formato_bitmap(mapa* map, FILE* fpout);

/*  gerar_mapa_formato_bitmap
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void gerar_mapa_formato_bitmap(mapa* map, bitmap* bm); //funcao auxiliar da funcao escrever mapa

/*  exportar_texto_formato_bitmap
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void exportar_texto_formato_bitmap(mapa* map, FILE* fpin, FILE* fpout);


#endif