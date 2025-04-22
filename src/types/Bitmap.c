/** Define um TAD representando um mapa de bits.
 * @file Bitmap.c
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#include <stdlib.h>
#include "types/Bitmap.h"

struct bitmap {
	unsigned int max_size;
	unsigned int length;
	unsigned char* contents;
};

unsigned char* bitmapGetContents(Bitmap* bm) {
	return bm->contents;
}

unsigned int bitmapGetMaxSize(Bitmap* bm) {
	return bm->max_size;
}

unsigned int bitmapGetLength(Bitmap* bm) {
	return bm->length;
}

Bitmap* bitmapInit(unsigned int max_size) {
	Bitmap* bm;
	bm = (Bitmap*)malloc(sizeof(Bitmap));
	unsigned int max_sizeInBytes=(max_size+7)/8;
	bm->contents=calloc(max_sizeInBytes, sizeof(char));
	bm->max_size=max_size;
	bm->length=0;
	return bm;
}

unsigned char bitmapGetBit(Bitmap* bm, unsigned int index) // index in bits
{
	return (bm->contents[index/8] >> (7-(index%8))) & 0x01;
}

static void bitmapSetBit(Bitmap* bm, unsigned int index, unsigned char bit) {
	// index/8 e' o indice do byte que contem o bit em questao
	// 7-(index%8) e' o deslocamento do bit em questao no byte
	bit=bit & 0x01;
	bit=bit<<(7-(index%8));
	bm->contents[index/8]= bm->contents[index/8] | bit;
}

void bitmapAppendLeastSignificantBit(Bitmap* bm, unsigned char bit) {
	bm->length++;
	bitmapSetBit(bm, bm->length-1, bit);
}

void bitmapLibera (Bitmap* bm){
	free(bm->contents);
	free(bm);
}




/**
 * Added by Carlos Daniel Albertino Vieira (cd_vieira@hotmail.com)
 */

void bitmapDump(Bitmap *bm, FILE* fpout){
	unsigned tam = bitmapGetLength(bm);
	tam = tam % 8 == 0 ? tam/8 : tam/8 + 1;
	/* O buffer do Bitmap (char*) foi alocado utilizando a função
	 * calloc, por isso apresenta um total de bits multiplo de 8.
	 * Isso fará a funcao fwrite escrever alguns bits a mais em
	 * relação ao tamanho real do Bitmap */
	fwrite(bm->contents, sizeof(unsigned char), tam, fpout);
}

void bitmapConcat(Bitmap *dest, Bitmap *source){
	const unsigned tam = bitmapGetLength(source);
	for(unsigned i=0; i<tam; i++){
		bitmapAppendLeastSignificantBit(dest, bitmapGetBit(source, i));
	}
}
