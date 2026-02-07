#pragma once

#include "core/HuffmanTree.h"
#include "utils/types/common.h"

typedef struct CompBody CompBody;

CompBody *compBody_from_huffmanTree(HuffmanTree *ht);
CompBody *compBody_destroy(CompBody *body);

size_t compBody_get_total_compressed_size_in_bits(CompBody *body);
size_t compBody_get_total_compressed_size_in_bytes(CompBody *body);

void compBody_encode_fileStream(CompBody *body, const char *filename);

void compBody_dump_into_fp(CompBody *body, FILE *fp);
void compBody_dump_into_fd(CompBody *body, int fd);
