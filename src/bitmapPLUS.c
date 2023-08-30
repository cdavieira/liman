#include "bitmap.h"
#include "bitmapPLUS.h"

void bitmapUnloadContents(bitmap* bm, FILE* fpout){
    if(bm && fpout){
        unsigned char* content = bitmapGetContents(bm);
        unsigned tam = bitmapGetLength(bm);
        fwrite(content, sizeof(unsigned char), tam?(1+tam/8):0, fpout);
        //lembre-se: o conteudo do bitmap (char*) foi alocado utilizando a função calloc, por isso apresenta um total de bits multiplo de 8. Por conta disso, a funcao fwrite irá escrever alguns bits a mais em relação ao tamanho atual do bitmap
    }
}

void bitmapUnloadContentsBinary(bitmap* bm, FILE* fpout){
    if(bm && fpout){
        unsigned tam = bitmapGetLength(bm);
        for(unsigned i=0;i<tam;i++){
        	fprintf(fpout, "%d", bitmapGetBit(bm, i));	
        }
        putchar('\n');
        //lembre-se: o conteudo do bitmap (char*) foi alocado utilizando a função calloc, por isso apresenta um total de bits multiplo de 8. Por conta disso, a funcao fwrite irá escrever alguns bits a mais em relação ao tamanho atual do bitmap
    }
}

void bitmapCatContents(bitmap* bm_dest, bitmap* bm_source){
    if(bm_dest && bm_source){
        unsigned tam = bitmapGetLength(bm_source);
        for(unsigned i=0;i<tam;i++){
            bitmapAppendLeastSignificantBit(bm_dest, bitmapGetBit(bm_source, i));
        }
    }
}
