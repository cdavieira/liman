#pragma once

#include "core/HuffmanTree.h"
#include "utils/container/Bitmap.h"
#include "utils/types/common.h"

typedef struct CompHeader CompHeader;
typedef struct BinaryWriter BinaryWriter;

CompHeader *compHeader_from_huffmanTree(HuffmanTree *huffmanTree);

/**
 * Read and parse the huffman tree stored in the compressed file
 *
 * The file cursor is advanced (on purpose) until 1 byte before the beginning
 * of the encoded message. This 1 byte tells how much padding was added in the
 * end of the encoded message to ensure byte alignment.
 *
 * For the compressed file, the bits are stored like this:
 *     00101101 = 0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
 *
 * It's worth mentioning that the huffmanTree was stored with proper byte
 * alignment.
 */
CompHeader *compHeader_from_bitmap(Bitmap *bm);

CompHeader *compHeader_destroy(CompHeader *header);

HuffmanTree *compHeader_get_huffmanTree(CompHeader *header);
Bitmap *compHeader_get_bitmap(CompHeader *header);
size_t compHeader_get_min_size_in_bits(CompHeader *header);
size_t compHeader_get_padding_size_in_bits(CompHeader *header);
size_t compHeader_get_total_size_in_bits(CompHeader *header);
size_t compHeader_get_total_size_in_bytes(CompHeader *header);
size_t compHeader_get_max_theoretical_size_in_bits(void);
size_t compHeader_get_max_theoretical_size_in_bytes(void);

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
 * 2. if a leaf node is encountered, print bit '1' followed by 8 bits. Otherwise
 * print bit '0'.
 * */
void compHeader_dump_into_binaryWriter(CompHeader *header,
                                       BinaryWriter *writer);
