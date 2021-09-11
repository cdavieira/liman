#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	puts("teste"); /* prints  */

	bitmap* bm=bitmapInit(10);
	printf("size=%d bits\n", bitmapGetMaxSize(bm));
	bitmapAppendLeastSignificantBit(bm, 1);
	bitmapAppendLeastSignificantBit(bm, 0);
	bitmapAppendLeastSignificantBit(bm, 0);
	bitmapAppendLeastSignificantBit(bm, 0);
	bitmapAppendLeastSignificantBit(bm, 1);
	bitmapAppendLeastSignificantBit(bm, 0);
	bitmapAppendLeastSignificantBit(bm, 0);
	bitmapAppendLeastSignificantBit(bm, 1);
	bitmapAppendLeastSignificantBit(bm, 0);
	bitmapAppendLeastSignificantBit(bm, 1);

	printf("%0xh\n", bitmapGetContents(bm)[0]);			
	printf("%0xh\n", bitmapGetContents(bm)[1]);
	printf("length=%0d\n", bitmapGetLength(bm));

	int i;
	for (i=0; i<bitmapGetLength(bm); i++) {
		printf("bit #%d = %0xh\n", i, bitmapGetBit(bm, i));
	}

    
    bitmapLibera(bm);
    
	puts("teste2");
	return EXIT_SUCCESS;

}
