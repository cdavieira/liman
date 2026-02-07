#pragma once

#include "utils/container/Bitmap.h"
#include "utils/types/common.h"

typedef struct HuffmanTree HuffmanTree;
typedef struct TreeCode TreeCode;

// Instantiation/destruction
HuffmanTree *huffmanTree_new(unsigned int ASCII, unsigned long weight,
                             HuffmanTree *left, HuffmanTree *right);
HuffmanTree *huffmanTree_destroy(HuffmanTree *hufftree);

// Getters
size_t huffmanTree_get_id(HuffmanTree *node);
unsigned int huffmanTree_get_ASCII(HuffmanTree *node);
unsigned long huffmanTree_get_weight(HuffmanTree *node);
Bitmap *huffmanTree_get_bitmap(HuffmanTree *node);
HuffmanTree *huffmanTree_get_left(HuffmanTree *hufftree);
HuffmanTree *huffmanTree_get_right(HuffmanTree *hufftree);
HuffmanTree *huffmanTree_get_child(HuffmanTree *map, unsigned lr);
TreeCode huffmanTree_get_code(HuffmanTree *hftree);

// Setters
HuffmanTree *huffmanTree_set_left(HuffmanTree *hufftree, HuffmanTree *child);
HuffmanTree *huffmanTree_set_right(HuffmanTree *hufftree, HuffmanTree *child);
HuffmanTree *huffmanTree_set_bitmap(HuffmanTree *hufftree, Bitmap *bm);

// To be or not to be
unsigned huffmanTree_is_leaf(HuffmanTree *hufftree);
int huffmanTree_compare_freq(HuffmanTree *t1, HuffmanTree *t2);

// Recursive calls
unsigned long huffmanTree_get_height(HuffmanTree *hufftree);
unsigned long huffmanTree_get_count(HuffmanTree *hufftree);
unsigned long huffmanTree_get_leaf_count(HuffmanTree *hufftree);
HuffmanTree *huffmanTree_descend(HuffmanTree *hufftree, const TreeCode *code);
void huffmanTree_gen_treeCodes(HuffmanTree *root);
HuffmanTree *huffmanTree_search_ASCII(HuffmanTree *hufftree, unsigned ASCII);
unsigned long huffmanTree_get_msg_size(HuffmanTree *hufftree);

/* prints the huffmanTree in the .dot format */
void huffmanTree_print(HuffmanTree *hufftree, FILE *fp);
void huffmanTree_print_codes(HuffmanTree *hufftree, FILE *fp);
