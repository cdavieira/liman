/** Define um TAD representando um mapa de bits.
 * @file bitmap.c
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#include <stdlib.h>
#include "container/Bitmap.h"

struct bitmap {
	unsigned int max_size;
	unsigned int length;
	unsigned char* contents;
};

unsigned char* bitmapGetContents(bitmap* bm) {
	return bm->contents;
}

unsigned int bitmapGetMaxSize(bitmap* bm) {
	return bm->max_size;
}

unsigned int bitmapGetLength(bitmap* bm) {
	return bm->length;
}

bitmap* bitmapInit(unsigned int max_size) {
	bitmap* bm;
	bm = (bitmap*)malloc(sizeof(bitmap));
	unsigned int max_sizeInBytes=(max_size+7)/8;
	bm->contents=calloc(max_sizeInBytes, sizeof(char));
	bm->max_size=max_size;
	bm->length=0;
	return bm;
}

unsigned char bitmapGetBit(bitmap* bm, unsigned int index) // index in bits
{
	return (bm->contents[index/8] >> (7-(index%8))) & 0x01;
}

static void bitmapSetBit(bitmap* bm, unsigned int index, unsigned char bit) {
	// index/8 e' o indice do byte que contem o bit em questao
	// 7-(index%8) e' o deslocamento do bit em questao no byte
	bit=bit & 0x01;
	bit=bit<<(7-(index%8));
	bm->contents[index/8]= bm->contents[index/8] | bit;
}

void bitmapAppendLeastSignificantBit(bitmap* bm, unsigned char bit) {
	bm->length++;
	bitmapSetBit(bm, bm->length-1, bit);
}

void bitmapLibera (bitmap* bm){
	free(bm->contents);
	free(bm);
}




/**
 * Added by Carlos Daniel Albertino Vieira (cd_vieira@hotmail.com)
 */

void bitmapUnloadContents(bitmap *bm, FILE* fpout){
	unsigned char* content = bitmapGetContents(bm);
	unsigned tam = bitmapGetLength(bm);
	/* O buffer do bitmap (char*) foi alocado utilizando a função
	 * calloc, por isso apresenta um total de bits multiplo de 8.
	 * Isso fará a funcao fwrite escrever alguns bits a mais em
	 * relação ao tamanho real do bitmap */
	fwrite(content, sizeof(unsigned char), tam? (1+tam/8) : 0, fpout);
}

void bitmapUnloadContentsBinary(bitmap *bm, FILE* fpout){
	/* O buffer do bitmap (char*) foi alocado utilizando a função calloc,
	 * por isso apresenta um total de bits multiplo de 8.
	 * Por conta disso, alguns bits a mais podem ser imprimidos. */
	unsigned const tam = bitmapGetLength(bm);
	for(unsigned i=0; i<tam; i++){
		fprintf(fpout, "%d", bitmapGetBit(bm, i));
	}
	putchar('\n');
}

void bitmapCatContents(bitmap *bm_dest, bitmap *bm_source){
	unsigned const tam = bitmapGetLength(bm_source);
	for(unsigned i=0;i<tam;i++){
		bitmapAppendLeastSignificantBit(bm_dest, bitmapGetBit(bm_source, i));
	}
}
