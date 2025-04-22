#include "types/HuffmanTree.h"
#include "types/Caracter.h"
#include "container/Tree.h"
#include <malloc.h>
#include <ctype.h>
#include <stdint.h>

HuffmanTree* huffmanTree_new(unsigned int ASCII, unsigned long weight, HuffmanTree* left, HuffmanTree* right){
	return (HuffmanTree*) tree_new((void*) caracter_new(ASCII, weight), (Tree*)left, (Tree*)right);
}

HuffmanTree* huffmanTree_destroy(HuffmanTree* hftree){
	return hftree ? (HuffmanTree*)tree_destroy((Tree*)hftree, (void*)caracter_destroy) : 0;
}




size_t huffmanTree_get_id(HuffmanTree* node){
	return tree_get_id((Tree*)node);
}

unsigned int huffmanTree_get_ASCII(HuffmanTree* node){
	return caracter_get_ASCII((Caracter*)tree_get_item((Tree*)node));
}

unsigned long huffmanTree_get_weight(HuffmanTree* node){
	return caracter_get_weight((Caracter*)tree_get_item((Tree*)node));
}

Bitmap* huffmanTree_get_bitmap(HuffmanTree* node){
	return caracter_get_bmap((Caracter*)tree_get_item((Tree*)node));
}

HuffmanTree* huffmanTree_get_left(HuffmanTree* hftree){
	return (HuffmanTree*) tree_get_left((Tree*) hftree);
}

HuffmanTree* huffmanTree_get_right(HuffmanTree* hftree){
	return (HuffmanTree*) tree_get_right((Tree*) hftree);
}

HuffmanTree* huffmanTree_get_child(HuffmanTree* hftree, unsigned lr){
	return (HuffmanTree*) tree_get_child((Tree*) hftree, lr);
}





HuffmanTree* huffmanTree_set_left(HuffmanTree* hftree, HuffmanTree* filho){
	return (HuffmanTree*)tree_set_left((Tree*) hftree, (Tree*) filho);
}

HuffmanTree* huffmanTree_set_right(HuffmanTree* hftree, HuffmanTree* filho){
	return (HuffmanTree*)tree_set_right((Tree*) hftree, (Tree*) filho);
}

HuffmanTree* huffmanTree_set_bitmap(HuffmanTree* hftree, Bitmap* bm){
	return (HuffmanTree*) tree_set_item((Tree*) hftree, (void*)caracter_set_bmap((Caracter*)tree_get_item((Tree*) hftree), bm));
}

unsigned huffmanTree_is_leaf(HuffmanTree* hftree){
	return tree_is_leaf((Tree*) hftree);
}




unsigned long huffmanTree_get_count(HuffmanTree* hftree){
	return tree_get_count((Tree*) hftree);
}

unsigned long huffmanTree_get_height(HuffmanTree* hftree){
	return tree_get_height((Tree*) hftree);
}

unsigned long huffmanTree_get_leaf_count(HuffmanTree* hftree){
	return tree_get_leaf_count((Tree*) hftree);
}

char* huffmanTree_find_path_str(HuffmanTree* arvore, HuffmanTree* node){
	return tree_find_path_str((Tree*) arvore, (Tree*) node);
}

int huffmanTree_find_path(HuffmanTree* root, HuffmanTree* node, unsigned long *codeLen, unsigned long *nodeCode){
	return tree_find_path((Tree*) root, (Tree*) node, codeLen, nodeCode);
}

HuffmanTree* huffmanTree_descend(HuffmanTree* hftree, unsigned long codeLen, unsigned long code){
	return (HuffmanTree*) tree_descend((Tree*) hftree, codeLen, code);
}

static unsigned huffman_compare_ASCII(void* hufftree1, void* ascii){
	return (huffmanTree_get_ASCII((HuffmanTree*)hufftree1)) == *((unsigned*)ascii);
}

HuffmanTree* huffmanTree_search_ASCII(HuffmanTree* hftree, unsigned ASCII){
	return (HuffmanTree*) tree_search((Tree*) hftree, (void*) &ASCII, huffman_compare_ASCII);
}

static unsigned long huffmanTree_get_msg_size_rec(HuffmanTree* hftree, unsigned long height){
	if(!hftree){
		return 0;
	}
	if(huffmanTree_is_leaf(hftree)){
		return height * huffmanTree_get_weight(hftree);
	}
	return huffmanTree_get_msg_size_rec(huffmanTree_get_left(hftree), height+1) + huffmanTree_get_msg_size_rec(huffmanTree_get_right(hftree), height+1);
}

unsigned long huffmanTree_get_msg_size(HuffmanTree* hftree){
	return huffmanTree_get_msg_size_rec(hftree, 0);
}

int huffmanTree_compare_freq(HuffmanTree* map1, HuffmanTree* map2){
	return huffmanTree_get_weight(map1) > huffmanTree_get_weight(map2);
}




void huffmanTree_print(HuffmanTree* hufftree, FILE* fp){
	tree_print((Tree*)hufftree, fp);
}

void huffmanTree_print_codes(HuffmanTree* hufftree, FILE* fp){
	HuffmanTree* t;
	char* nodeCode;
	const char* code;
	const char* notfound = "not encoded";
	ssize_t nodeid;
	for(unsigned ch = 0; ch<256; ch++){
		t = huffmanTree_search_ASCII(hufftree, ch);
		nodeCode = huffmanTree_find_path_str(hufftree, t);
		code = nodeCode? nodeCode : notfound;
		nodeid = t ? huffmanTree_get_id(t) : -1;
		if(isprint(ch)){
			fprintf(fp, "(Tree %03ld) %c: %s\n", nodeid, ch, code);
		}
		else{
			fprintf(fp, "(Tree %03ld) %u: %s\n", nodeid, ch, code);
		}
		free(nodeCode);
	}
}
