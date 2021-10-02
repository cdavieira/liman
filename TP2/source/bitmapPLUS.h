#ifndef _bitmapPLUS
#define _bitmapPLUS
#include <stdio.h>

typedef struct map bitmap;

/*  bitmapUnloadContents
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void bitmapUnloadContents(bitmap* bm, FILE* fpout);

//bm_dest deve possuir tamanho suficiente para armazenar o seu conteudo somado ao conteudo da fonte após a concatenação
/*  bitmapCatContents
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void bitmapCatContents(bitmap* bm_dest, bitmap* bm_source);

#endif