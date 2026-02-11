#pragma once

#include "utils/types/common.h"

typedef struct CompBody CompBody;
typedef struct HuffmanTree HuffmanTree;
typedef struct BinaryWriter BinaryWriter;

CompBody *compBody_from_huffmanTree(HuffmanTree *ht);
CompBody *compBody_destroy(CompBody *body);

size_t compBody_get_total_compressed_size_in_bits(CompBody *body);
size_t compBody_get_total_compressed_size_in_bytes(CompBody *body);

void compBody_encode(CompBody *body, const char *filename,
                     BinaryWriter *writer);
