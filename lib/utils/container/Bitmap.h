#pragma once

/**
 * Define um TAD representando um mapa de bits.
 * @file bitmap.h
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br) */
typedef struct bitmap Bitmap;
unsigned char *bitmapGetContents(const Bitmap *bm);
unsigned int bitmapGetMaxSize(const Bitmap *bm);
unsigned int bitmapGetLength(const Bitmap *bm);
Bitmap *bitmapInit(unsigned int max_size);
unsigned char bitmapGetBit(const Bitmap *bm, unsigned int index);
void bitmapAppendLeastSignificantBit(Bitmap *bm, unsigned char bit);
void bitmapLibera(Bitmap *bm);

/**
 * Added by Carlos Daniel Albertino Vieira (cd_vieira@hotmail.com) */
#include "utils/types/common.h"
void bitmapPrint(const Bitmap *bm, FILE *fpout);
void bitmapDump(Bitmap *bm, FILE *fpout);
void bitmapConcat(Bitmap *dest, const Bitmap *source);
Bitmap *bitmapClone(Bitmap *src);
unsigned char bitmapGetByte(const Bitmap *bm, unsigned int index);
