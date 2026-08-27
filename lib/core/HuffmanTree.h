#pragma once

#include "core/HuffmanCode.h"
#include "utils/types/common.h"

typedef struct HuffmanTree HuffmanTree;

// Instantiation/destruction
HuffmanTree *huffmanTree_new(unsigned int key,
                             unsigned long weight,
                             HuffmanTree *left,
                             HuffmanTree *right);
HuffmanTree *huffmanTree_destroy(HuffmanTree *hufftree);

// Getters
size_t huffmanTree_get_id(HuffmanTree *node);
unsigned int huffmanTree_get_key(HuffmanTree *node);
unsigned long huffmanTree_get_weight(HuffmanTree *node);
Code huffmanTree_get_code(HuffmanTree *hftree);
HuffmanCode *huffmanTree_get_value(HuffmanTree *hftree);
HuffmanTree *huffmanTree_get_left(HuffmanTree *hufftree);
HuffmanTree *huffmanTree_get_right(HuffmanTree *hufftree);
HuffmanTree *huffmanTree_get_child(HuffmanTree *map, unsigned lr);

// Setters
HuffmanTree *huffmanTree_set_left(HuffmanTree *hufftree, HuffmanTree *child);
HuffmanTree *huffmanTree_set_right(HuffmanTree *hufftree, HuffmanTree *child);

// To be or not to be
unsigned huffmanTree_is_leaf(HuffmanTree *hufftree);
int huffmanTree_compare_freq(HuffmanTree *t1, HuffmanTree *t2);

// Recursive calls
unsigned long huffmanTree_get_height(HuffmanTree *hufftree);
unsigned long huffmanTree_get_count(HuffmanTree *hufftree);
unsigned long huffmanTree_get_leaf_count(HuffmanTree *hufftree);
void huffmanTree_gencodes(HuffmanTree *root);
HuffmanTree *huffmanTree_search_key(HuffmanTree *hufftree, unsigned key);
unsigned long huffmanTree_get_msg_size(HuffmanTree *hufftree);

/* prints the huffmanTree in the .dot format */
void huffmanTree_printfp_dot(HuffmanTree *hufftree, FILE *fp);
void huffmanTree_printfp_codes(HuffmanTree *hufftree, FILE *fp);
