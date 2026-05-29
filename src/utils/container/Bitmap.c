/** Define um TAD representando um mapa de bits.
 * @file Bitmap.c
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#include "utils/container/Bitmap.h"
#include "platform/mem.h"

#include <stdio.h>
#include <string.h>

struct bitmap {
  unsigned int max_size;
  unsigned int length;
  unsigned char *contents;
};

unsigned char *bitmapGetContents(const Bitmap *bm) { return bm->contents; }

unsigned int bitmapGetMaxSize(const Bitmap *bm) { return bm->max_size; }

unsigned int bitmapGetLength(const Bitmap *bm) { return bm->length; }

Bitmap *bitmapInit(unsigned int max_size) {
  Bitmap *bm;
  bm = (Bitmap *)mem_alloc(sizeof(Bitmap));
  unsigned int max_sizeInBytes = (max_size + 7) / 8;
  bm->contents = mem_zalloc(max_sizeInBytes * sizeof(char));
  bm->max_size = max_size;
  bm->length = 0;
  return bm;
}

unsigned char bitmapGetBit(const Bitmap *bm,
                           unsigned int index) // index in bits
{
  return (bm->contents[index / 8] >> (7 - (index % 8))) & 0x01;
}

static void bitmapSetBit(Bitmap *bm, unsigned int index, unsigned char bit) {
  // index/8 e' o indice do byte que contem o bit em questao
  // 7-(index%8) e' o deslocamento do bit em questao no byte
  bit = bit & 0x01;
  bit = bit << (7 - (index % 8));
  bm->contents[index / 8] = bm->contents[index / 8] | bit;
}

void bitmapAppendLeastSignificantBit(Bitmap *bm, unsigned char bit) {
  bm->length++;
  bitmapSetBit(bm, bm->length - 1, bit);
}

void bitmapLibera(Bitmap *bm) {
  if (bm) {
    mem_free(bm->contents);
  }
  mem_free(bm);
}

/**
 * Added by Carlos Daniel Albertino Vieira (cd_vieira@hotmail.com)
 */

void bitmapPrint(const Bitmap *bm, FILE *fpout) {
  const unsigned tam = bitmapGetLength(bm);
  for (unsigned i = 0; i < tam; i++) {
    unsigned char byte = bitmapGetBit(bm, i);
    putc(byte ? '1' : '0', fpout);
  }
}

void bitmapDump(Bitmap *bm, FILE *fpout) {
  unsigned tam = bitmapGetLength(bm);
  tam = tam % 8 == 0 ? tam / 8 : tam / 8 + 1;
  /* O tamanho real do buffer do Bitmap (char*) é multiplo de 8.
   * Isso fará a funcao fwrite escrever alguns bits a mais em
   * relação ao length do Bitmap */
  fwrite(bm->contents, sizeof(unsigned char), tam, fpout);
}

void bitmapConcat(Bitmap *dest, const Bitmap *source) {
  const unsigned tam = bitmapGetLength(source);
  for (unsigned i = 0; i < tam; i++) {
    bitmapAppendLeastSignificantBit(dest, bitmapGetBit(source, i));
  }
}

Bitmap *bitmapClone(Bitmap *src) {
  Bitmap *bm = bitmapInit(src->max_size);
  bm->length = src->length;
  memcpy(bm->contents, src->contents, (src->max_size + 7) / 8);
  return bm;
}

unsigned char bitmapGetByte(const Bitmap *bm,
                            unsigned int index) // index in bits
{
  unsigned char ascii = 0;
  unsigned char bit;
  for (int i = 7, j = 0; i >= 0; i--, j++) {
    bit = bitmapGetBit(bm, index + j);
    bit <<= i;
    ascii |= bit;
  }
  return ascii;
}
