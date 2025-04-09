#pragma once

#include <stdio.h>
#include "container/Bitmap.h"
typedef struct HuffmanTree HuffmanTree;

//Instantiation/destruction
HuffmanTree* huffmanTree_new(unsigned int ASCII, unsigned long weight, HuffmanTree* left, HuffmanTree* right);
HuffmanTree* huffmanTree_destroy(HuffmanTree* hufftree);

//Getters
size_t huffmanTree_get_id(HuffmanTree* node);
unsigned int huffmanTree_get_ASCII(HuffmanTree* node);
unsigned long huffmanTree_get_weight(HuffmanTree* node);
Bitmap* huffmanTree_get_bitmap(HuffmanTree* node);
HuffmanTree* huffmanTree_get_left(HuffmanTree* hufftree);
HuffmanTree* huffmanTree_get_right(HuffmanTree* hufftree);

//Setters
HuffmanTree* huffmanTree_set_left(HuffmanTree* hufftree, HuffmanTree* filho);
HuffmanTree* huffmanTree_set_right(HuffmanTree* hufftree, HuffmanTree* filho);
HuffmanTree* huffmanTree_set_bitmap(HuffmanTree* hufftree, Bitmap* bm);

//To be or not to be
unsigned huffmanTree_is_leaf(HuffmanTree* hufftree);
/* h1.weight > h2.weight ? 1 : 0 */
int huffmanTree_compare_freq(HuffmanTree* t1, HuffmanTree* t2);

//Recursive calls
unsigned long huffmanTree_get_height(HuffmanTree* hufftree);
unsigned long huffmanTree_get_count(HuffmanTree* hufftree);
unsigned long huffmanTree_get_leaf_count(HuffmanTree* hufftree);
char* huffmanTree_find_path(HuffmanTree* arvore, HuffmanTree* node);
HuffmanTree* huffmanTree_descend(HuffmanTree* hufftree, unsigned long codeLen, unsigned long code);
HuffmanTree* huffmanTree_search_ASCII(HuffmanTree* hufftree, unsigned ASCII);
/**
 * calculate the minimum number of bits needed to store the msg encoded by this tree.
 * 
 * this function can only be called by the compressor. The huffmanTree decoded
 * by the decompressor lacks the 'weights' of each byte. Therefore, if the
 * decompressor calls this function, it will always return 0.
 * */
unsigned long huffmanTree_get_msg_size(HuffmanTree* hufftree);

/* prints the huffmanTree in the .dot format */
void huffmanTree_print(HuffmanTree* hufftree, FILE* fp);
void huffmanTree_print_codes(HuffmanTree* hufftree, FILE* fp);
