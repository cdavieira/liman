#pragma once

#include <sys/types.h>
#include "core/HuffmanTree.h"



typedef struct InputBytes InputBytes;
InputBytes* inputBytes_new(char* f);
InputBytes* inputBytes_destroy(InputBytes* in);
unsigned char* inputBytes_getBytes(InputBytes* in);
ssize_t inputBytes_getSize(InputBytes* in);



typedef struct ByteFrequency ByteFrequency;
ByteFrequency* byteFreq_new(InputBytes* in);
ByteFrequency* byteFreq_destroy(ByteFrequency* f);
int byteFreq_compare_callback(const void* b1, const void* b2);
unsigned char byteFreq_getByte(ByteFrequency* f);
size_t byteFreq_getSize(ByteFrequency* f);




/* ordinary huffman algorithm */
HuffmanTree* huffman(ByteFrequency* f);

/*
 * tree encoding convention:
 * 0 for non-leaf nodes
 * 1 for leaf nodes
 *
 * a tree 't' is printed in the following way:
 * print(t)
 * print_recursive(t->left)
 * print_recursive(t->right)
 *
 * if 't' is a leaf node, then 1 byte gets printed next, which is the ASCII
 * character stored in that leaf node
 *
 * Summary:
 * 1. transverse the tree recursively
 * 2. if a leaf node is encountered, print bit '1' followed by 8 bits. Otherwise print bit '0'.
 * */
size_t dumpHuffmanTree(HuffmanTree* map, FILE* fpout);

/* rewrite the input using the huffman encoding */
size_t dumpHuffmanEncodedText(HuffmanTree* map, InputBytes* in, FILE* fpout);
