#pragma once

#include "CompHeader.h"
#include "utils/types/common.h"

typedef struct CompReader CompReader;
typedef struct CompReaderOutput {
  size_t input_total_size_bytes;
  size_t output_total_size_bytes;
  size_t output_min_size_bits;
  size_t output_pad_bits;
} CompReaderOutput;

CompReader *compReader_new(const char *filename);
CompReader *compReader_destroy(CompReader *reader);

CompHeader *compReader_get_header(CompReader *reader);

/**
 * Translate the contents of a .comp file according to its huffmanTree
 *
 * Algorithm:
 * 1. read one bit
 * 2. check if that bit take us to a leaf node of the huffmanTree
 * 3. If yes, then print the associated caracter with the leaf Node. Otherwise,
 * continue until no bits are left
 */
CompReaderOutput compReader_translate(CompReader *reader, const char *filename);
