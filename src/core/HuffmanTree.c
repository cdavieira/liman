#include "core/HuffmanTree.h"
#include "core/Caracter.h"
#include "container/Tree.h"
#include <malloc.h>
#include <ctype.h>

static unsigned long huffmanTree_get_msg_size_rec(HuffmanTree* map, unsigned long height);




HuffmanTree* huffmanTree_new(unsigned int ASCII, unsigned long weight, HuffmanTree* left, HuffmanTree* right){
	return (HuffmanTree*) tree_new((void*) caracter_new(ASCII, weight), (Tree*)left, (Tree*)right);
}

HuffmanTree* huffmanTree_destroy(HuffmanTree* map){
	return map ? (HuffmanTree*)tree_destroy((Tree*)map, (void*)caracter_destroy) : 0;
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

HuffmanTree* huffmanTree_get_left(HuffmanTree* map){
	return (HuffmanTree*) tree_get_left((Tree*) map);
}

HuffmanTree* huffmanTree_get_right(HuffmanTree* map){
	return (HuffmanTree*) tree_get_right((Tree*) map);
}





HuffmanTree* huffmanTree_set_left(HuffmanTree* map, HuffmanTree* filho){
	return (HuffmanTree*)tree_set_left((Tree*) map, (Tree*) filho);
}

HuffmanTree* huffmanTree_set_right(HuffmanTree* map, HuffmanTree* filho){
	return (HuffmanTree*)tree_set_right((Tree*) map, (Tree*) filho);
}

HuffmanTree* huffmanTree_set_bitmap(HuffmanTree* map, Bitmap* bm){
	return (HuffmanTree*) tree_set_item((Tree*) map, (void*)caracter_set_bmap((Caracter*)tree_get_item((Tree*) map), bm));
}

unsigned huffmanTree_is_leaf(HuffmanTree* map){
	return tree_is_leaf((Tree*) map);
}




unsigned long huffmanTree_get_count(HuffmanTree* map){
	return tree_get_count((Tree*) map);
}

unsigned long huffmanTree_get_height(HuffmanTree* map){
	return tree_get_height((Tree*) map);
}

unsigned long huffmanTree_get_leaf_count(HuffmanTree* map){
	return tree_get_leaf_count((Tree*) map);
}

char* huffmanTree_find_path(HuffmanTree* arvore, HuffmanTree* node){
	return tree_find_path_str((Tree*) arvore, (Tree*) node);
}

HuffmanTree* huffmanTree_descend(HuffmanTree* map, unsigned long codeLen, unsigned long code){
	return (HuffmanTree*) tree_descend((Tree*) map, codeLen, code);
}

static unsigned huffman_compare_ASCII(void* hufftree1, void* ascii){
	return (huffmanTree_get_ASCII((HuffmanTree*)hufftree1)) == *((unsigned*)ascii);
}

HuffmanTree* huffmanTree_search_ASCII(HuffmanTree* map, unsigned ASCII){
	return (HuffmanTree*) tree_search((Tree*) map, (void*) &ASCII, huffman_compare_ASCII);
}

unsigned long huffmanTree_get_msg_size(HuffmanTree* map){
	return huffmanTree_get_msg_size_rec(map, 0);
}

static unsigned long huffmanTree_get_msg_size_rec(HuffmanTree* map, unsigned long height){
	if(!map){
		return 0;
	}
	if(huffmanTree_is_leaf(map)){
		return height * huffmanTree_get_weight(map);
	}
	return huffmanTree_get_msg_size_rec(huffmanTree_get_left(map), height+1) + huffmanTree_get_msg_size_rec(huffmanTree_get_right(map), height+1);
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
	for(unsigned letra = 0; letra<256; letra++){
		t = huffmanTree_search_ASCII(hufftree, letra);
		nodeCode = huffmanTree_find_path(hufftree, t);
		code = nodeCode? nodeCode : notfound;
		nodeid = t ? huffmanTree_get_id(t) : -1;
		if(isprint(letra)){
			fprintf(fp, "(Tree %03ld) %c: %s\n", nodeid, letra, code);
		}
		else{
			fprintf(fp, "(Tree %03ld) %u: %s\n", nodeid, letra, code);
		}
		free(nodeCode);
	}
}
