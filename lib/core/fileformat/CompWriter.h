#pragma once

#include "core/HuffmanTree.h"

#include "utils/types/common.h"

typedef struct CompWriter CompWriter;
typedef struct CompWriterStatistics {
  float compression_rate;
  size_t compressed_size;
  size_t uncompressed_size;
} CompWriterStatistics;

CompWriter *compWriter_from_huffmanTree(HuffmanTree *ht);
CompWriter *compWriter_destroy(CompWriter *writer);

void compWriter_set_inputfile(CompWriter *writer, const char *inputfile);

/**
 * Dump the contents of the header and the body to filename
 * */
void compWriter_dump(CompWriter *writer, const char *outputfile);

CompWriterStatistics compWriter_get_statistics(CompWriter *writer);
size_t compWriter_get_total_compressed_size_in_bytes(CompWriter *writer);
size_t compWriter_get_total_uncompressed_size_in_bytes(CompWriter *writer);
float compWriter_get_compression_rate(CompWriter *writer);
