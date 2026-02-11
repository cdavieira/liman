#pragma once

#include "core/HuffmanTree.h"
#include "utils/types/common.h"

typedef struct BinaryWriter BinaryWriter;

BinaryWriter *binWriter_new(size_t block_size);
BinaryWriter *binWriter_destroy(BinaryWriter *writer);

void binWriter_open(BinaryWriter *writer, const char *filename);
void binWriter_close(BinaryWriter *writer);

int binWriter_is_open(BinaryWriter *writer);
void binWriter_flush(BinaryWriter *writer);

void binWriter_write_bit(BinaryWriter *writer, int bit);
void binWriter_write_byte(BinaryWriter *writer, unsigned char byte);
void binWriter_write_bitmap(BinaryWriter *writer, const Bitmap *bm);
void binWriter_write_treeCode(BinaryWriter *writer, TreeCode code);
