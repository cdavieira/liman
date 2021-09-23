#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	puts("teste"); /* prints  */

	bitmap* bm=bitmapInit(7);
	printf("size=%d bits\n", bitmapGetMaxSize(bm));
	// bitmapAppendLeastSignificantBit(bm, 1);	//bit mais signficativo
	// bitmapAppendLeastSignificantBit(bm, 1);	//retorna c, pois: {} {} _ _, 2³ + 2² = 12 (c)
	bitmapAppendLeastSignificantBit(bm, 0);	//bit mais signficativo
	bitmapAppendLeastSignificantBit(bm, 1);
	bitmapAppendLeastSignificantBit(bm, 1);
	bitmapAppendLeastSignificantBit(bm, 0); //ate aqui a gnt so tem 1 hexadecimal (0x60) e nos 4 bits seguintes tem se (0x01)
	bitmapAppendLeastSignificantBit(bm, 0);
	bitmapAppendLeastSignificantBit(bm, 0); 
	bitmapAppendLeastSignificantBit(bm, 1);


	//bitmapAppendLeastSignificantBit(bm, 1); //bit menos significativo //a cada 4 bits isso marca a escrita de um hexadecimal | com 8 bits é possível escrever ff valores  
	// bitmapAppendLeastSignificantBit(bm, 1); //a partir daqui inicia-se a escrita de um novohexadecimal | só existem dois bits nesse hexadecimal e eles sao os 2 bits mais da esquerda, ou seja: {a} {b} _ _
	// bitmapAppendLeastSignificantBit(bm, 1);
	// bitmapAppendLeastSignificantBit(bm, 1);

	//um numero hexdecimal é armazenado como nibble (4 bits) -> os dois da esquerda sao os primeiros a serem preenchidos e dps os ultimos 2
	//o par de hexadecimais (8 bits) leva 2 nibbles

	// char c = (char)(bitmapGetContents(bm)[0] | (unsigned char)0x0);
/* 	
	char c = 0x01;
	c = c<<6;
	printf("%c\n", c);

	printf("%0xh\n", bitmapGetContents(bm)[0]);			
	printf("%0xh\n", bitmapGetContents(bm)[1]);
	printf("length=%0d\n", bitmapGetLength(bm));
*/
	int i;
	for (i=0; i<bitmapGetLength(bm); i++) {
		printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	}
	printf("%u\n", *(unsigned*)bitmapGetContents(bm));

/* 	FILE* bin = fopen("./respostas/binario.bin", "wb");
	fwrite((void*)bitmapGetContents(bm), sizeof(unsigned char), bitmapGetLength(bm), bin);
	fclose(bin);
	bin = fopen("./respostas/binario.bin", "rb");
	char a;
	fread((void*) &a, sizeof(char), 1, bin);
	fclose(bin);
	putchar(a);
    
    bitmapLibera(bm);
    
	puts("teste2"); */
	return EXIT_SUCCESS;

}
