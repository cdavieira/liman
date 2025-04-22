#pragma once

/**
 * Define um TAD representando um mapa de bits.
 * @file bitmap.h
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br) */
typedef struct bitmap Bitmap;
unsigned char* bitmapGetContents(Bitmap* bm);
unsigned int bitmapGetMaxSize(Bitmap* bm);
unsigned int bitmapGetLength(Bitmap* bm);
Bitmap* bitmapInit(unsigned int max_size);
unsigned char bitmapGetBit(Bitmap* bm, unsigned int index);
void bitmapAppendLeastSignificantBit(Bitmap* bm, unsigned char bit);
void bitmapLibera(Bitmap* bm);

/**
 * Added by Carlos Daniel Albertino Vieira (cd_vieira@hotmail.com) */
#include <stdio.h>
void bitmapDump(Bitmap *bm, FILE* fpout);
void bitmapConcat(Bitmap *dest, Bitmap *source);
