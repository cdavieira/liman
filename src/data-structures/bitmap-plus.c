#include "data-structures/bitmap-plus.h"
#include "data-structures/bitmap.h" //bitmapGetContents, bitmapGetLength, bitmapAppendLeastSignificantBit, bitmapGetBit

void bitmapUnloadContents(bitmap* bm, FILE* fpout){
	if(!bm || !fpout){
		return ;
	}

	unsigned char* content = bitmapGetContents(bm);
	unsigned tam = bitmapGetLength(bm);
	/* lembre-se: o conteudo do bitmap (char*) foi alocado utilizando a função
	 * calloc, por isso apresenta um total de bits multiplo de 8. Por conta
	 * disso, a funcao fwrite irá escrever alguns bits a mais em relação ao
	 * tamanho atual do bitmap */
	fwrite(content, sizeof(unsigned char), tam?(1+tam/8):0, fpout);
}

void bitmapUnloadContentsBinary(bitmap* bm, FILE* fpout){
	if(!bm || !fpout){
		return ;
	}

	/* lembre-se: o conteudo do bitmap (char*) foi alocado utilizando a
	 * função calloc, por isso apresenta um total de bits multiplo de 8.
	 * Por conta disso, a funcao fwrite irá escrever alguns bits a mais em
	 * relação ao tamanho atual do bitmap */
	unsigned const tam = bitmapGetLength(bm);
	for(unsigned i=0;i<tam;i++){
		fprintf(fpout, "%d", bitmapGetBit(bm, i));	
	}
	putchar('\n');
}

void bitmapCatContents(bitmap* bm_dest, bitmap* bm_source){
	if(!bm_dest && !bm_source){
		return ;
	}

	unsigned const tam = bitmapGetLength(bm_source);
	for(unsigned i=0;i<tam;i++){
		bitmapAppendLeastSignificantBit(bm_dest, bitmapGetBit(bm_source, i));
	}
}
