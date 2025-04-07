/**
 * Define um TAD representando um mapa de bits.
 * @file bitmap.h
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br) */

#pragma once

typedef struct bitmap bitmap;
unsigned char* bitmapGetContents(bitmap* bm);
unsigned int bitmapGetMaxSize(bitmap* bm);
unsigned int bitmapGetLength(bitmap* bm);
bitmap* bitmapInit(unsigned int max_size);
unsigned char bitmapGetBit(bitmap* bm, unsigned int index);
void bitmapAppendLeastSignificantBit(bitmap* bm, unsigned char bit);
void bitmapLibera (bitmap* bm);

/**
 * Added by Carlos Daniel Albertino Vieira (cd_vieira@hotmail.com)
 */

#include <stdio.h>
/** grava o conteúdo do bitmap num arquivo apontado por fpout */
void bitmapUnloadContents(bitmap *bm, FILE* fpout);
/** grava o conteúdo em binário do bitmap num arquivo apontado por fpout */
void bitmapUnloadContentsBinary(bitmap  *bm, FILE* fpout);
/** concatena os conteúdos de dois bitmaps, armazenando o conteudo de bm_source em bm_dest */
void bitmapCatContents(bitmap *bm_dest, bitmap *bm_source);
