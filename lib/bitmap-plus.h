#pragma once

#include <stdio.h>

typedef struct map bitmap;

/*  bitmapUnloadContents
    Descrição: grava o conteúdo do bitmap num arquivo apontado por fpout
    
    Input: bitmap a ser escrito e ponteiro para saída
    
    Output: 
    
    Pré condição: argumentos fornecidos devem ser não nulos
    
    Pós condicao: -
*/
void bitmapUnloadContents(bitmap* bm, FILE* fpout);

/*  bitmapUnloadContentsBinary
    Descrição: grava o conteúdo em binário do bitmap num arquivo apontado por fpout
    
    Input: bitmap a ser escrito e ponteiro para saída
    
    Output: 
    
    Pré condição: argumentos fornecidos devem ser não nulos
    
    Pós condicao: -
*/
void bitmapUnloadContentsBinary(bitmap* bm, FILE* fpout);

/*  bitmapCatContents
    Descrição: concatena os conteúdos de dois bitmaps, armazenando o conteudo de bm_source em bm_dest
    
    Input: bitmap destino onde será armazenado seu conteúdo e o conteúdo de bm_source, que consiste no bitmap a ser concatenado
    
    Output: -
    
    Pré condição: bm_dest deve possuir tamanho suficiente para armazenar o seu conteudo somado ao conteudo da fonte após a concatenação
    
    Pós condicao: -
*/
void bitmapCatContents(bitmap* bm_dest, bitmap* bm_source);
