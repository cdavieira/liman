#include "core/decompress.h"
#include "core/HuffmanTree.h"
#include "container/Bitmap.h"
#include "utils/bits.h"
#include "utils/file.h"
#include <stdlib.h>
#include <assert.h>

HuffmanTree* read_huffmanTree(FILE* fpin){
	Bitmap *bm = read_bitmap(fpin);
	assert(bm);
	unsigned index = 0;
	HuffmanTree *map = bitmap2huffmanTree(bm, &index);
	bitmapLibera(bm);
	return map;
}

Bitmap* read_bitmap(FILE* fpin){
	const size_t bm_size = count_tree_bits(fpin);
	if(!bm_size){
		return NULL;
	}

	Bitmap* bm = bitmapInit(bm_size);
	char c = 0;
	unsigned long bits = 0;
	while(bits < bm_size){
		c = fgetc(fpin);
		for(int j=7; j>=0; j--){
			bitmapAppendLeastSignificantBit(bm, bits_bitAt(c, j));
			bits++;
			if(bits == bm_size){
				break;
			}
		}
	}
	return bm;
}

HuffmanTree* bitmap2huffmanTree(Bitmap* bm, unsigned *index){
	if(*index >= bitmapGetLength(bm)){
		return NULL;
	}

	// index gets incremented only after the function call returns
	unsigned char bit = bitmapGetBit(bm, (*index)++);
	HuffmanTree* map = NULL;
	if(bit){ //Leaf node
		unsigned char ascii = 0;
		for(unsigned i=0;i<8; i++){
			bit = bitmapGetBit(bm, (*index)++);
			bit <<= (7-i);
			ascii |= bit;
		}
		map = huffmanTree_new(ascii, 0, 0, 0);
	}
	else{ //Non leaf node
		map = huffmanTree_new(0, 0, 0, 0);
		map = huffmanTree_set_left(map, bitmap2huffmanTree(bm, index));
		map = huffmanTree_set_right(map, bitmap2huffmanTree(bm, index));
	}

	return map;
}

size_t read_msg(FILE* fpin, FILE* fpout, HuffmanTree* hufftree){
	HuffmanTree* t;
	unsigned char bit;
	unsigned char ch;
	unsigned code = 0;
	unsigned codeLen = 0;
	size_t nBits = 0;
	ssize_t nBytes = 0;
	ssize_t lastByte = file_get_remaining_nbytes(fpin);
	for(char c = fgetc(fpin); !feof(fpin); c = fgetc(fpin)){
		nBytes++;
		for(int i=7; i>=0; i--){
			nBits++;
			bit = bits_bitAt(c, i);
			code = (code << 1) | bit;
			codeLen++;
			t = huffmanTree_descend(hufftree, codeLen, code);
			if(!t || !huffmanTree_is_leaf(t)){
				continue;
			}

			//we've encountered a leaf node! Hooray!
			ch = huffmanTree_get_ASCII(t);
#ifdef DEBUG
			printf("debug:read_msg:%d -> %d\n", code, ch);
#endif
			code = 0;
			codeLen = 0;

			//If two or more NULLS exist in the last byte (which
			//is fairly uncommon), then the decompression of the
			//message will fail :(
			//The reason for this bug is that '\0' can be used
			//(just like any other byte) throughout the compressed
			//message to encode data (this is expected). Therefore
			//we need to find a way of knowing when to stop
			//reading the file, since '\0' isn't by itself a
			//signal to do so anymore. My workaround for this
			//problem is to check if we're on the last byte of the
			//file or not. If we are in the last byte and we read
			//a '\0', then we stop reading.  But, what if '\0' was
			//used in the last byte of the file just to encode
			//information and there's one additional '\0' which is
			//the EOF? Well, thats the issue!
			if(ch == '\0' && nBytes == lastByte){
				break;
			}
			if(fpout){
				fprintf(fpout, "%c", ch);
			}
		}
	}
	return nBits;
}

size_t count_tree_bits(FILE* fpin){
	rewind(fpin);

	//why do we initialize leafs to 1 instead of 0?
	//To understand this, reason through the following two cases:
	//
	//* Case 1) If the bit for the root node is 0, then we are expected to
	//read 2 leafs.
	//
	//* Case 2) If the bit for the root node is 1, then we are expected to
	//read 1 leaf node (which is the root node itself).
	//
	//Therefore, our initial guess for the number of leafs yet to be read
	//is 1 because of Case 2). If we verify that the bit for the root node
	//is 0, then the loop routine will increase the number of leafs to 2,
	//which covers Case 1)!
	int leafs = 1; //leafs yet to be read

	//after reading a leaf node, we have to read the following 8 bits.
	//'byteBits' tracks how many of those 8 bits remain yet to be read.
	size_t byteBits = 0; //char bits yet to be read

	size_t totalBits = 0;
	char c;
	int bit;

	//pause and ponder how the following loop works for a
	//sequence of bits such as:
	//1) 01000000_00100000_000
	//2) 1
	while(!feof(fpin)){
		c = fgetc(fpin);
		for(int i=7; i>=0; i--){
			bit = bits_bitAt(c, i);
			if(byteBits == 0){
				//Not currently reading a byte
				if(bit){
					byteBits = 8;
				}
				else{
					leafs++;
				}
			}
			else{
				//Currently reading a byte
				byteBits--;
				if(byteBits == 0){
					leafs--;
				}
			}
			totalBits++;
			if(leafs == 0){
				goto quit;
			}
		}
	}
quit:
	rewind(fpin);
	return totalBits;
}
