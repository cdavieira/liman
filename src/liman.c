#include "liman.h"
#include "types/HuffmanTree.h"
#include "utils/bits.h"
#include "utils/file.h"
#include "types/TreeList.h"
#include "types/Bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

struct InputBytes {
	unsigned char* bytes;
	ssize_t nbytes;
};

struct ByteFrequency {
	unsigned byte;
	size_t count;
};

struct FileHeader {
	Bitmap* bm;
	size_t treeHeight;
	size_t treeNodeCount;
	size_t treeLeafCount;
	size_t treeMinBits;
	size_t treePadBits;
	size_t treeTotalBits; //msgTotalBits % 8 == 0
};

struct FileBody {
	Bitmap* bm;
	size_t msgMinBits;
	size_t msgPadBits;
	size_t msgTotalBits; //msgTotalBits % 8 == 0
};





static void fill_header(HuffmanTree* hf, Bitmap* header);
static HuffmanTree* header2huffmanTree(Bitmap* header);
static HuffmanTree* rec_header2huffmanTree(Bitmap* header, unsigned *index);
static size_t liman_count_header(FILE* fpin);
static void gencodes(HuffmanTree* root, unsigned codeLen, unsigned long nodeCode);
static HuffmanTree* decode_next_symb(HuffmanTree* node, Bitmap* bm, size_t *idx, size_t sz);
static unsigned long get_null_code_length(HuffmanTree* root);





// Core
void compress(char* inputfile){
	InputBytes* in = inputBytes_new(inputfile);
	ByteFrequency* freq = byteFreq_new(in);
	HuffmanTree* hufftree = huffmanAlgorithm(freq);

	FileHeader* hdr = limanHeader_new(hufftree);
	FileBody* body = limanBody_new(hufftree, in);
	char *outputfile = file_add_ext(file_rem_path(inputfile), ".comp");
	if(liman_dump(hdr, body, outputfile) != 0){
		exit(EXIT_FAILURE);
	}

	size_t oldsz = in->nbytes;
	size_t newsz = (hdr->treeTotalBits + body->msgTotalBits + 8) >> 3; //in bytes
	float diff = ((double)oldsz - (double)newsz)/oldsz;
	printf("compression finished: %s saved as %s!\n", inputfile, outputfile);
	printf("from %zu bytes to %zu bytes (%.2f%% compressed)\n", oldsz, newsz, 100.0f*diff);

	free(outputfile);
	limanBody_destroy(body);
	limanHeader_destroy(hdr);
	huffmanTree_destroy(hufftree);
	byteFreq_destroy(freq);
	inputBytes_destroy(in);
}

void decompress(char* inputfile){
	FILE *fpin = fopen(inputfile, "rb");
	if(fpin == NULL){
		exit(EXIT_FAILURE);
	}

	FileHeader* hdr = limanHeader_read(fpin); 
	HuffmanTree* root = header2huffmanTree(hdr->bm);

	char* outputfile = file_add_pref(file_rem_ext(inputfile), "unhuffman-");
	FILE *fpout = fopen(outputfile, "wb");
	if(fpout == NULL){
		exit(EXIT_FAILURE);
	}

#ifdef TWO_PASS
	FileBody* body = limanBody_read(root, fpin); 
	if(liman_decode(root, body, fpout) != 0){
		exit(EXIT_FAILURE);
	}
	limanBody_destroy(body);
#else
	if(liman_decode_single_pass(root, fpin, fpout) != 0){
		exit(EXIT_FAILURE);
	}
#endif

	size_t oldsz = file_get_nbytes(fpin); //in bytes
	size_t newsz = file_get_nbytes(fpout); //in bytes
	printf("decompression finished: %s saved as %s!\n", inputfile, outputfile);
	printf("from %zu bytes to %zu bytes\n", oldsz, newsz);

	fclose(fpout);
	free(outputfile);
	huffmanTree_destroy(root);
	limanHeader_destroy(hdr);
	fclose(fpin);
}

void analyze(char* compfile, int opts){
	FILE *fpi = fopen(compfile, "rb");
	if(fpi == NULL){
		fprintf(stderr, "huhman: cant open %s\n", compfile);
		return ;
	}

	FileHeader* hdr = limanHeader_read(fpi); 
	HuffmanTree* root = header2huffmanTree(hdr->bm);

	if(opts & HUHMAN_HEADER){
		FileHeader* hdr_mirror = limanHeader_new(root);
		printf(
		  "About the Huffman Tree:\n"
		  "Height: %zu\n"
		  "Nodes : %zu\n"
		  "Leafs : %zu\n",
		  hdr_mirror->treeHeight,
		  hdr_mirror->treeNodeCount,
		  hdr_mirror->treeLeafCount);
		printf(
		  "About the header:\n"
		  "Tree size     (read/expected): %zu bits/%lu bits\n"
		  "Padding added (read/expected): %zu bits/%lu bits\n"
		  "Total         (read/expected): %zu bytes/%lu bytes\n",
		  hdr->treeMinBits, hdr_mirror->treeMinBits,
		  hdr->treePadBits, hdr_mirror->treePadBits,
		  hdr_mirror->treeTotalBits/8, hdr_mirror->treeTotalBits/8);
		limanHeader_destroy(hdr_mirror);
	}
	if(opts & HUHMAN_BODY){
		FileBody* body = limanBody_read(root, fpi); 
		printf(
		  "About the body:\n"
		  "Size:          %zu bits\n"
		  "Padding added: %zu bits\n"
		  "Total:         %zu bytes\n",
		  body->msgMinBits,
		  body->msgPadBits,
		  body->msgTotalBits/8);
		limanBody_destroy(body);
	}
	if(opts & HUHMAN_CODES){
		char* tmpName = file_add_ext(compfile, ".txt");
		char* outfile = file_add_pref(tmpName, "huhman-");
		FILE* fpo = fopen(outfile, "w");
		if(fpo){
			huffmanTree_print_codes(root, fpo);
			printf("Tree codes: %s\n", outfile);
			fclose(fpo);
		}
		free(tmpName);
		free(outfile);
	}
	if(opts & HUHMAN_PDF){
		char* tmpName = file_add_ext(compfile, ".dot");
		char* outfile = file_add_pref(tmpName, "huhman-");
		FILE* fpo = fopen(outfile, "w");
		if(fpo){
			huffmanTree_print(root, fpo);
			printf("Tree dotfile: %s\n", outfile);
			printf("Tip: run 'dot -Tpdf %s -o %s.pdf' and generate its pdf!\n", outfile, outfile);
			fclose(fpo);
		}
		free(tmpName);
		free(outfile);
	}

	fclose(fpi);
	huffmanTree_destroy(root);
	limanHeader_destroy(hdr);
}






//InputBytes
InputBytes* inputBytes_new(char* f){
	FILE *fp = fopen(f, "rb");
	if(fp == NULL){
		return NULL;
	}

	InputBytes *in = malloc(sizeof(InputBytes));
	in->nbytes = file_get_nbytes(fp);
	in->bytes = calloc(in->nbytes+1, sizeof(unsigned char));

	for(int i=0; i<in->nbytes; i++){
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





//ByteFrequency
ByteFrequency* byteFreq_new(InputBytes* in){
	ByteFrequency* freq = calloc(256, sizeof(ByteFrequency));
	for(int i=0; i<256; i++){
		freq[i].count = 0;
		freq[i].byte = i;
	}
	freq[0].count = 1;
	for(int i=0; i<in->nbytes; i++){
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





HuffmanTree* huffmanAlgorithm(ByteFrequency* f){
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
		printf("%u: %lu\n", huffmanTree_get_ASCII(debug_tree), huffmanTree_get_weight(debug_tree));
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
	treelist_destroy(lc);
	gencodes(hufftree, 0, 0);
	return hufftree;
}






FileHeader* limanHeader_new(HuffmanTree* t){
	FileHeader* h = malloc(sizeof(FileHeader));
	h->treeHeight = huffmanTree_get_height(t);
	h->treeNodeCount = huffmanTree_get_count(t);
	h->treeLeafCount = huffmanTree_get_leaf_count(t);
	h->treeMinBits = h->treeNodeCount + 8*h->treeLeafCount; //each Leaf encodes 1 byte, therefore 8 additional bits
	h->treePadBits = h->treeMinBits % 8 == 0? 0 : 8 - (h->treeMinBits % 8);
	h->treeTotalBits = h->treeMinBits + h->treePadBits;
	h->bm = bitmapInit(h->treeTotalBits);
	fill_header(t, h->bm);
	return h;
}

FileHeader* limanHeader_read(FILE* fp){
	size_t minBits = liman_count_header(fp);
	if(!minBits){
		return NULL;
	}
	FileHeader* hdr = malloc(sizeof(FileHeader));
	hdr->bm = bitmapInit(minBits);
	hdr->treeMinBits = minBits;
	hdr->treePadBits = minBits % 8 == 0 ? 0 : 8 - minBits % 8;
	hdr->treeTotalBits = minBits + hdr->treePadBits;

	size_t bits = 0;
	for(char c = fgetc(fp); !feof(fp); c = fgetc(fp)){
		for(int j=7; j>=0; j--){
			bitmapAppendLeastSignificantBit(hdr->bm, bits_bitAt(c, j));
			if(bits++ == minBits){
				goto stop;
			}
		}
	}
stop:
	return hdr;
}

FileHeader* limanHeader_destroy(FileHeader* hdr){
	bitmapLibera(hdr->bm);
	free(hdr);
	return NULL;
}

FileBody* limanBody_new(HuffmanTree* root, InputBytes* in){
	Bitmap* bmapLookup[256];
	HuffmanTree* t;
	for(unsigned i=0; i<256; i++){
		t = huffmanTree_search_ASCII(root, i);
		bmapLookup[i] = t ? huffmanTree_get_bitmap(t) : NULL;
	}

	FileBody* body = malloc(sizeof(FileBody));
	body->msgMinBits = huffmanTree_get_msg_size(root);
	body->msgPadBits = body->msgMinBits % 8 == 0 ? 0 : 8 - body->msgMinBits % 8;
	body->msgTotalBits = body->msgMinBits + body->msgPadBits;
	body->bm = bitmapInit(body->msgTotalBits);
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
			bitmapConcat(body->bm, bmapCode);
		}
	}
	return body;
}

FileBody* limanBody_read(HuffmanTree* root, FILE* fp){
	char padBits = fgetc(fp);
	if((padBits < 0) || padBits > 7){
		return NULL;
	}

	FileBody* body = malloc(sizeof(FileBody));
	body->msgTotalBits = file_get_remaining_nbytes(fp) * 8;
	body->msgPadBits = padBits;
	body->msgMinBits = body->msgTotalBits - body->msgPadBits;
	body->bm = bitmapInit(body->msgMinBits);
	size_t nBit = 0;
	for(char c = fgetc(fp); !feof(fp); c = fgetc(fp)){
		for(int i=7; i>=0; i--){
			bitmapAppendLeastSignificantBit(body->bm, bits_bitAt(c, i));
			if(nBit++ == body->msgMinBits){
				goto stop;
			}
		}
	}
stop:
	return body;
}

FileBody* limanBody_destroy(FileBody* body){
	bitmapLibera(body->bm);
	free(body);
	return NULL;
}

int liman_dump(FileHeader* header, FileBody* body, const char* filename){
	FILE *fp = fopen(filename, "wb");
	if(fp == NULL){
		return -1;
	}
	bitmapDump(header->bm, fp);
	fputc((char) body->msgPadBits, fp);
	bitmapDump(body->bm, fp);
	fclose(fp);
	return 0;
}

int liman_decode(HuffmanTree* root, FileBody* body, FILE* fp){
	if(fp == NULL){
		return -1;
	}

	size_t i = 0;
	size_t sz = bitmapGetLength(body->bm) - get_null_code_length(root);
	HuffmanTree* t;
	unsigned ch;
	while((t = decode_next_symb(root, body->bm, &i, sz)) != NULL){
		fputc(huffmanTree_get_ASCII(t), fp);
	}
	return 0;
}

int liman_decode_single_pass(HuffmanTree* root, FILE* fpin, FILE* fpout){
	char padBits = fgetc(fpin);
	if((padBits < 0) || padBits > 7){
		return -1;
	}

	size_t msgTotalBits = file_get_remaining_nbytes(fpin) * 8;
	size_t msgPadBits = padBits;
	size_t msgMinBits = msgTotalBits - msgPadBits - get_null_code_length(root);

#ifdef DEBUG
	unsigned code = 0;
	unsigned codeLen = 0;
#endif
	size_t nBit = 0;
	unsigned char bit;
	HuffmanTree* node = root;
	for(char c = fgetc(fpin); !feof(fpin); c = fgetc(fpin)){
		for(int i=7; i>=0; i--){
			nBit++;
			bit = bits_bitAt(c, i);
#ifdef DEBUG
			code = (code << 1) | bit;
			codeLen++;
#endif
			node = huffmanTree_get_child(node, bit);
			if(huffmanTree_is_leaf(node)){
#ifdef DEBUG
				printf("debug:liman_decode_1pass:");
				for(int i=codeLen-1; i>=0; i--){
					printf("%c", '0' + ((code >> i) & 1));
				}
				printf("-> %u (%zu/%zu bit)\n", huffmanTree_get_ASCII(node), nBit,  msgMinBits);
				code = 0;
				codeLen = 0;
#endif
				fputc(huffmanTree_get_ASCII(node), fpout);
				node = root;
			}

			if(nBit == msgMinBits){
				goto stop;
			}

		}
	}
stop:
	return 0;
}





static void fill_header(HuffmanTree* hf, Bitmap* header){
	if(!huffmanTree_is_leaf(hf)){
		bitmapAppendLeastSignificantBit(header, 0);
		fill_header(huffmanTree_get_left(hf), header);
		fill_header(huffmanTree_get_right(hf), header);
		return ;
	}
	bitmapAppendLeastSignificantBit(header, 1);
	unsigned char ch = huffmanTree_get_ASCII(hf);
	for(int i=7; i>=0; i--){
		bitmapAppendLeastSignificantBit(header, bits_bitAt(ch, i));
	}
}

static HuffmanTree* header2huffmanTree(Bitmap* header){
	unsigned idx = 0;
	return  rec_header2huffmanTree(header, &idx);
}

static HuffmanTree* rec_header2huffmanTree(Bitmap* bm, unsigned *index){
	if(*index >= bitmapGetLength(bm)){
		return NULL;
	}

	unsigned char bit = bitmapGetBit(bm, (*index)++);
	HuffmanTree* node = NULL;
	if(bit){ //Leaf node
		unsigned char ascii = 0;
		for(int i=7; i>=0; i--){
			bit = bitmapGetBit(bm, (*index)++);
			bit <<= i;
			ascii |= bit;
		}
		node = huffmanTree_new(ascii, 0, 0, 0);
	}
	else{ //Non leaf node
		node = huffmanTree_new(0, 0, 0, 0);
		node = huffmanTree_set_left(node, rec_header2huffmanTree(bm, index));
		node = huffmanTree_set_right(node, rec_header2huffmanTree(bm, index));
	}

	return node;
}

// Count how many bits were used to encode the huffmanTree in the compressed file
static size_t liman_count_header(FILE* fpin){
	if(fpin == NULL){
		return 0;
	}

	rewind(fpin);

	/* Why leafs=1 instead of leafs=0 ?
	 *
	 * I)   The first bit of the file tells us the type of the root node.
	 * II)  If the first bit is 0, then the root node is a nonleaf.
	 *      Otherwise, its a leaf node.
	 * III) The tree has at least 1 leaf node (which might be the root node)
	 * IV)  A nonleaf node introduces 2 leaf nodes. A leafnode introduces 0.
	 *
	 * Because of III), we have to initially assume that the number of
	 * leafs yet to be read is 1. If the root node is a leafnode, then all
	 * good! But, if the root node is a non leafnode, then now we have to
	 * read 2 leaf nodes. This can be done by incrementing our previous
	 * 'leaf count' by 1 and continuing our search. This logic applies to
	 * all nodes of the tree!
	 * */
	int leafs = 1; //leafs yet to be read

	//after reading a leaf node, we have to read the following 8 bits.
	size_t byteBits = 0; //tracks how many of those 8 bits remain yet to be read.
	size_t totalBits = 0;
	// char c;
	int bit;

	//pause and ponder how the following loop works for a sequence of bits such as:
	//* 01000000_00100000_000XXXXX
	//* 1XXXXXXX
	for(char c = fgetc(fpin); !feof(fpin); c = fgetc(fpin)){
	// while(!feof(fpin)){
		// c = fgetc(fpin);
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

//static and recursive
static void gencodes(HuffmanTree* root, unsigned codeLen, unsigned long nodeCode){
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
	gencodes(huffmanTree_get_left(root), codeLen+1, (nodeCode << 1) | 0);
	gencodes(huffmanTree_get_right(root), codeLen+1, (nodeCode << 1) | 1);
}

static HuffmanTree* decode_next_symb(HuffmanTree* node, Bitmap* bm, size_t *idx, size_t sz){
#ifdef DEBUG
	unsigned code = 0;
	unsigned codeLen = 0;
#endif
	unsigned char bit;
	HuffmanTree* leaf = NULL;
	while(*idx < sz){
		bit = bitmapGetBit(bm, (*idx)++);
#ifdef DEBUG
		code = (code << 1) | bit;
		codeLen++;
#endif
		node = huffmanTree_get_child(node, bit);
		if(huffmanTree_is_leaf(node)){
			leaf = node;
			break;
		}
	}

#ifdef DEBUG
	if(leaf){
		printf("debug:liman_decode:");
		for(int i=codeLen-1; i>=0; i--){
			printf("%c", '0' + ((code >> i) & 1));
		}
		printf("-> %u (%zu/%zu bit)\n", huffmanTree_get_ASCII(leaf), *idx+1, sz);
	}
#endif
	return leaf;
}

static unsigned long get_null_code_length(HuffmanTree* root){
	HuffmanTree* nullhf = huffmanTree_search_ASCII(root, '\0');
	unsigned long codeLen = 0;
	unsigned long nodeCode = 0;
	if(huffmanTree_find_path(root, nullhf, &codeLen, &nodeCode) != 0){
	}
	return codeLen;
}
