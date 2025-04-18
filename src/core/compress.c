#include "core/compress.h"
#include "core/HuffmanTree.h"
#include "container/TreeList.h"
#include "container/Bitmap.h"
#include "utils/bits.h"
#include "utils/file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct InputBytes {
	unsigned char* bytes;
	ssize_t nbytes;
};

struct ByteFrequency {
	unsigned char byte;
	size_t count;
};


static void huffmanTree_gencodes(HuffmanTree* root, unsigned codeLen, unsigned long nodeCode);
static void huffmanTree_fillBitmap(HuffmanTree* map, Bitmap* bm);



HuffmanTree* huffman(ByteFrequency* f){
	unsigned nonZeroBytes = 0;
	for(int i=0; i<256; i++){
		if(f[i].count > 0){
			nonZeroBytes++;
		}
	}

	TreeList* lc = treelist_new(nonZeroBytes);
	for(int i=255; i>=0; i--){
		if(f[i].count > 0){
			lc = treelist_shift(lc, huffmanTree_new(f[i].byte, f[i].count, 0, 0));
		}
	}

#ifdef DEBUG
	printf("debug:huffman:BEGIN:list to be processed\n");
	HuffmanTree* debug_tree;
	for(int i=0; i<treelist_get_size(lc); i++){
		debug_tree = treelist_get_tree(lc, i);
		printf("%d: %lu\n", huffmanTree_get_ASCII(debug_tree), huffmanTree_get_weight(debug_tree));
	}
	printf("debug:huffman:END\n");
#endif

	HuffmanTree *ltree;
	HuffmanTree *rtree;
	unsigned long w1;
	unsigned long w2;
	for(size_t sz = nonZeroBytes; sz > 1; sz--){
		ltree = treelist_unshift(lc);
		rtree = treelist_unshift(lc);
		w1 = huffmanTree_get_weight(ltree);
		w2 = huffmanTree_get_weight(rtree);
		lc = treelist_add_inorder(lc, huffmanTree_new(0, w1+w2, ltree, rtree));
	}

	HuffmanTree* hufftree = treelist_unshift(lc);
	lc = treelist_destroy(lc);
	huffmanTree_gencodes(hufftree, 0, 0);
	return hufftree;
}

size_t dumpHuffmanTree(HuffmanTree* map, FILE* fpout){
	unsigned long nNodes = huffmanTree_get_count(map);
	unsigned long nLeafs = huffmanTree_get_leaf_count(map);
	unsigned long nMinBits = nNodes + 8*nLeafs; //each Leaf encodes 1 byte, therefore 8 additional bits
	unsigned long nExceedingBits = nMinBits % 8;
	unsigned long nPadBits = 8 - nExceedingBits; //padBits: zero'd bits added to ensure byte alignment
	unsigned long nTotalBits = nExceedingBits? nMinBits + nPadBits : nMinBits;

#ifdef DEBUG
	printf("debug:dumpHuffmanTree:tree minsize = %lu bits\n", nMinBits);
	printf("debug:dumpHuffmanTree:tree padbits = %lu bits\n", nPadBits);
#endif

	Bitmap* bm = bitmapInit(nTotalBits);
	huffmanTree_fillBitmap(map, bm);
	bitmapDump(bm, fpout);
	bitmapLibera(bm);

	return nTotalBits;
}

size_t dumpHuffmanEncodedText(HuffmanTree* root, InputBytes* in, FILE* fpout){
	Bitmap* bmapLookup[256];
	HuffmanTree* t;
	for(unsigned i=0; i<256; i++){
		t = huffmanTree_search_ASCII(root, i);
		bmapLookup[i] = t ? huffmanTree_get_bitmap(t) : NULL;
	}

	unsigned long size = huffmanTree_get_msg_size(root);
	Bitmap *bmapMsg = bitmapInit(size);
	Bitmap *bmapCode = NULL;
	for(ssize_t i=0; i<=in->nbytes; i++){
		bmapCode = bmapLookup[in->bytes[i]];
#ifdef DEBUG
		printf("debug:dumpHuffmanTree:(byte %zd/%zd) %d -> ", i, in->nbytes, in->bytes[i]);
		for(int i=0; i<bitmapGetLength(bmapCode); i++){
			printf("%d", bitmapGetBit(bmapCode, i));
		}
		putchar('\n');
#endif
		if(bmapCode){
			bitmapConcat(bmapMsg, bmapCode);
		}
#ifdef DEBUG
		else{
			printf("debug:dumpHuffmanTree:failed to encode char %zd\n", i);
			assert(0);
		}
#endif
	}

	bitmapDump(bmapMsg, fpout);
	bitmapLibera(bmapMsg);

	//adding padbits
	size = size % 8 == 0 ? size : size + (8 - size % 8);
	return size;
}





//static and recursive
static void huffmanTree_gencodes(HuffmanTree* root, unsigned codeLen, unsigned long nodeCode){
	if(!root){
		return ;
	}
	//nodeCode is a byte-encoded number which takes the root tree to one of its leaf nodes.
	//For example:
	//for nodeCode=101 (codeLen=3), then: leaf = root->right->left->right
	//for nodeCode=10001 (codeLen=5), then: leaf = root->right->left->left->left->right
	//
	//Since nodeCode is a 64bit number, the maximum height of the huffmanTree becomes 64.
	//
	//In the original project, nodeCode was built as a string in this
	//function, which allowed the generation of arbitrary code lengths.
	assert(codeLen < 64);
	if(huffmanTree_is_leaf(root)){
		Bitmap* bm = bitmapInit(codeLen);
		for(int i=codeLen; i>0; i--){
			bitmapAppendLeastSignificantBit(bm, (nodeCode >> (i-1)) & 1);
		}
		huffmanTree_set_bitmap(root, bm);
	}
	huffmanTree_gencodes(huffmanTree_get_left(root), codeLen+1, (nodeCode << 1) | 0);
	huffmanTree_gencodes(huffmanTree_get_right(root), codeLen+1, (nodeCode << 1) | 1);
}

static void huffmanTree_fillBitmap(HuffmanTree* map, Bitmap* bm){
	if(!huffmanTree_is_leaf(map)){
		bitmapAppendLeastSignificantBit(bm, 0);
		huffmanTree_fillBitmap(huffmanTree_get_left(map), bm);
		huffmanTree_fillBitmap(huffmanTree_get_right(map), bm);
		return ;
	}
	bitmapAppendLeastSignificantBit(bm, 1);
	unsigned char ch = huffmanTree_get_ASCII(map);
	for(int i=7; i>=0; i--){
		bitmapAppendLeastSignificantBit(bm, bits_bitAt(ch, i));
	}
}





//InputBytes
InputBytes* inputBytes_new(char* f){
	FILE *fp = fopen(f, "rb");

	ssize_t nbytes = file_get_nbytes(fp);
	assert(nbytes > 0);

	InputBytes *in = malloc(sizeof(InputBytes));
	in->bytes = calloc(nbytes, sizeof(char));
	in->nbytes = nbytes;

	for(int i=0; i<nbytes; i++){
		in->bytes[i] = fgetc(fp);
	}

	fclose(fp);

	return in;
}

InputBytes* inputBytes_destroy(InputBytes* in){
	free(in->bytes);
	free(in);
	return NULL;
}

ssize_t inputBytes_getSize(InputBytes* in){
	return in->nbytes;
}





//ByteFrequency
ByteFrequency* byteFreq_new(InputBytes* in){
	ByteFrequency* freq = calloc(256, sizeof(ByteFrequency));
	for(int i=0; i<256; i++){
		freq[i].count = 0;
		freq[i].byte = i;
	}
	for(int i=0; i<=in->nbytes; i++){
		freq[in->bytes[i]].count++;
	}
	qsort(freq, 256, sizeof(ByteFrequency), byteFreq_compare_callback);
	return freq;
}

ByteFrequency* byteFreq_destroy(ByteFrequency* f){
	free(f);
	return NULL;
}

int byteFreq_compare_callback(const void* b1, const void* b2){
	return ((ByteFrequency*)b1)->count - ((ByteFrequency*)b2)->count;
}
