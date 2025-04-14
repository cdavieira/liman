#include "liman.h"
#include "core/HuffmanTree.h"
#include "core/compress.h"
#include "core/decompress.h"
#include "container/Bitmap.h"
#include "utils/file.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

void compress(char* inputfile){
	InputBytes* in = inputBytes_new(inputfile);
	ByteFrequency* freq = byteFreq_new(in);
	HuffmanTree* hufftree = huffman(freq);
	char *outputfile = file_add_ext(file_rem_path(inputfile), ".comp");
	FILE *fpout = fopen(outputfile, "wb");
	size_t treesz = dumpHuffmanTree(hufftree, fpout); //in bits
	size_t textsz = dumpHuffmanEncodedText(hufftree, in, fpout); //in bits
	size_t oldsz = inputBytes_getSize(in); //in bytes
	size_t newsz = ((treesz+textsz)>>3); //in bytes
	printf("compression finished: %s saved as %s!\n", inputfile, outputfile);
	printf("from %zu bytes to %zu bytes (%.2f%%)\n", oldsz, newsz, (100.0f*newsz)/oldsz);

	fclose(fpout);
	free(outputfile);
	hufftree = huffmanTree_destroy(hufftree);
	freq = byteFreq_destroy(freq);
	in = inputBytes_destroy(in);
}

void decompress(char* inputfile){
	FILE *fpin = fopen(inputfile, "rb");
	HuffmanTree *map = read_huffmanTree(fpin);
	assert(map);

	char* outputfile = file_add_pref(file_rem_ext(inputfile), "unhuffman-");
	FILE *fpout = fopen(outputfile, "wb");
	read_msg(fpin, fpout, map);

	size_t oldsz = file_get_nbytes(fpin); //in bytes
	size_t newsz = file_get_nbytes(fpout); //in bytes
	printf("decompression finished: %s saved as %s!\n", inputfile, outputfile);
	printf("from %zu bytes to %zu bytes\n", oldsz, newsz);

	fclose(fpout);
	free(outputfile);
	huffmanTree_destroy(map);
	fclose(fpin);
}


/*
 * Padding bits are there to ensure byte alignment.
 * */
typedef struct {
	HuffmanTree* tree;

	// TREE
	size_t treeHeight;
	size_t treeNodeCount;
	size_t treeLeafCount;
	size_t treeExpectedSize; //in bits
	size_t treeExpectedPad; //in bits
	size_t treeSizeRead; //in bits
	size_t treePadRead; //in bits

	// MSG
	size_t msgSizeRead; //in bits
	size_t msgPadRead; //in bits
} HuffmanFile ;

void analyze(char* compfile){
	HuffmanFile f;
	memset(&f, 0, sizeof(HuffmanFile));
	FILE *fp = fopen(compfile, "rb");

	//Tree Info
	f.tree = read_huffmanTree(fp);
	f.treeHeight = huffmanTree_get_height(f.tree);
	f.treeNodeCount = huffmanTree_get_count(f.tree);
	f.treeLeafCount = huffmanTree_get_leaf_count(f.tree);
	f.treeExpectedSize = f.treeNodeCount + 8*f.treeLeafCount; //each Leaf encodes 1 byte, therefore 8 additional bits
	f.treeExpectedPad = f.treeExpectedSize % 8 == 0 ? 0 : 8 - f.treeExpectedSize % 8; //padBits: zero'd bits added to ensure byte alignment
	f.treeSizeRead = count_tree_bits(fp);
	f.treePadRead = f.treeSizeRead % 8 == 0 ? 0 : 8 - f.treeSizeRead % 8;

	//Msg Info
	bitmapLibera(read_bitmap(fp)); //advance the file until the beginning of the msg
	ssize_t startMsgIdx = ftell(fp);
	f.msgSizeRead = read_msg(fp, NULL, f.tree);
	if(fseek(fp, startMsgIdx, SEEK_SET) != 0){
		perror("analyze: fseek error");
		return;
	}
	size_t totalMsg = 0;
	for(fgetc(fp); !feof(fp); fgetc(fp)){
		totalMsg += 8;
	}
	f.msgPadRead = totalMsg ? totalMsg - f.msgSizeRead : 0;

	fclose(fp);

	//Dot representation
	char* dotoutput = file_add_ext(compfile, ".dot");
	fp = fopen(dotoutput, "w");
	huffmanTree_print(f.tree, fp);
	fclose(fp);

	//Codes generated
	char* codeoutput = file_add_ext(compfile, ".txt");
	fp = fopen(codeoutput, "w");
	huffmanTree_print_codes(f.tree, fp);
	fclose(fp);

	//Dump info
	size_t msgTotalSizeBits = f.msgSizeRead + f.msgPadRead;
	size_t msgTotalSizeBytes = msgTotalSizeBits/8;
	size_t treeTotalSizeBits = f.treeSizeRead + f.treePadRead;
	size_t treeTotalSizeBytes = treeTotalSizeBits/8;
	size_t fileTotalSizeBits = msgTotalSizeBits + treeTotalSizeBits;
	size_t fileTotalSizeBytes = msgTotalSizeBytes + treeTotalSizeBytes;
	printf("Tree height: %lu\n", f.treeHeight);
	printf("Tree node count: %lu\n", f.treeNodeCount);
	printf("Tree leaf count: %lu\n", f.treeLeafCount);
	printf("Tree size (expected/read): %lu bits/%lu bits\n", f.treeExpectedSize, f.treeSizeRead);
	printf("Tree pad (expected/read): %lu bits/%lu bits\n", f.treeExpectedPad, f.treePadRead);
	printf("Tree total size: %lu bytes (%lu bits)\n", treeTotalSizeBytes, treeTotalSizeBits);
	printf("Msg size: %lu bytes (%lu bits)\n", f.msgSizeRead/8, f.msgSizeRead);
	printf("Msg pad: %lu bits\n", f.msgPadRead);
	printf("Msg total size: %lu bytes (%lu bits)\n", msgTotalSizeBytes, msgTotalSizeBits);
	printf("File size: %lu bytes (%lu bits)\n", fileTotalSizeBytes, fileTotalSizeBits);
	printf("Tree codes: %s\n", codeoutput);
	printf("Tree dotfile: %s\n", dotoutput);
	printf("Tip: run 'dot -Tpdf %s -o %s.pdf' and generate its pdf!\n", dotoutput, dotoutput);

	free(codeoutput);
	free(dotoutput);
	huffmanTree_destroy(f.tree);
}
