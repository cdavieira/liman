#include "core/fileformat/CompReader.h"
#include "core/HuffmanTree.h"
#include "core/fileformat/CompHeader.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/posix/BinaryWriter.h"
#include "platform/posix/FileStream.h"
#include "platform/process.h"
#include "utils/bits.h"
#include "utils/container/Bitmap.h"
#include "utils/container/Tree.h"

#include <stdio.h>

struct CompReader {
  const char *filename;
  FileStream *fs;
};

typedef struct CallbackData2 {
  HuffmanTree *root;
  HuffmanTree *tree;
  BinaryWriter *writer;
  CompReaderOutput output;
} CallbackData2;

typedef struct CallbackData {
  // leafs yet to be read
  int leafs;

  // after reading a leaf node, we have to read the following 8 bits.
  // tracks how many of those 8 bits remain yet to be read.
  size_t remainingLeafBits;

  // the total number of bits read
  size_t totalBits;
} CallbackData;

// static size_t liman_count_header(FILE *fpin);
static size_t compReader_count_header(CompReader *reader);
static int compReader_count_header_callback(int bit, void *data);
static Bitmap *compReader_read_header_bitmap(CompReader *reader);
static int compReader_read_header_bitmap_callback(int bit, void *data);
static int compReader_translate_callback(int bit, void *data);
static unsigned long get_null_code_length(HuffmanTree *root);

CompReader *compReader_new(const char *filename) {
  CompReader *reader = mem_alloc(sizeof(struct CompReader));
  reader->filename = filename;
  reader->fs = fs_new(1024 * 1024);
  fs_open_file(reader->fs, filename);
  return reader;
}

CompReader *compReader_destroy(CompReader *reader) {
  fs_destroy(reader->fs);
  return mem_free(reader);
}

CompHeader *compReader_get_header(CompReader *reader) {
  Bitmap *bm = compReader_read_header_bitmap(reader);
  return compHeader_from_bitmap(bm);
}

CompReaderOutput compReader_translate(CompReader *reader,
                                      const char *filename) {
  CompReaderOutput res = {.sizeBits = 0, .sizeBytes = 0};

  CompHeader *header = compReader_get_header(reader);
  HuffmanTree *root = compHeader_get_huffmanTree(header);

  // Reading pad bits
  int padBits = fs_get_byte(reader->fs);
  if ((padBits < 0) || padBits > 7) {
    process_abort("Body pad field out of range");
  }
  res.padBits = padBits;

  // Advancing pad field
  fs_next_byte(reader->fs);

  // Writing to output
  BinaryWriter *writer = binWriter_new(4096 * 8); // 4 KiB
  binWriter_open(writer, filename);

  CallbackData2 *data = mem_alloc(sizeof(CallbackData2));

  data->writer = writer;
  data->root = root;
  data->tree = root;
  data->output = res;

  size_t msgTotalBits = bits_fromBytes(fs_remaining_bytes(reader->fs));
  size_t msgMinBits = msgTotalBits - padBits - get_null_code_length(root);

  fs_do_next_bits(reader->fs, msgMinBits, data, compReader_translate_callback);

  res = data->output;
  res.sizeBytes = bits_toBytes(res.sizeBits + res.padBits);

  binWriter_destroy(writer);
  data = mem_free(data);

  compHeader_destroy(header);

  return res;
}

static size_t compReader_count_header(CompReader *reader) {
  fs_rewind(reader->fs);

  CallbackData *data = mem_alloc(sizeof(CallbackData));

  /* Why leafs=1 instead of leafs=0 ?
   *
   * I)   The first bit of the file tells us the type of the root node.
   * II)  If the first bit is 0, then the root node is a nonleaf.
   *      Otherwise, its a leaf node.
   * III) The tree has at least 1 leaf node (which might be the root node)
   * IV)  A nonleaf node introduces 2 leaf nodes. A leafnode introduces 0.
   *
   * Because of III), we have to initially assume that the number of
   * leafs yet to be read is 1. If the root node is a leafnode, then all
   * good! But, if the root node is a non leafnode, then now we have to
   * read 2 leaf nodes. This can be done by incrementing our previous
   * 'leaf count' by 1 and continuing our search. This logic applies to
   * all nodes of the tree!
   * */
  data->leafs = 1;
  data->totalBits = 0;
  data->remainingLeafBits = 0;

  fs_loop_over_all_bits(reader->fs, reader->filename, data,
                        compReader_count_header_callback);

  size_t totalBits = data->totalBits;

  data = mem_free(data);

  fs_rewind(reader->fs);

  return totalBits;
}

static int compReader_count_header_callback(int bit, void *data) {
  CallbackData *d = data;

  int not_reading_byte = d->remainingLeafBits == 0;
  if (not_reading_byte) {
    if (bit) {
      d->remainingLeafBits = 8;
    } else {
      d->leafs++;
    }
  } else {
    d->remainingLeafBits--;
    if (d->remainingLeafBits == 0) {
      d->leafs--;
    }
  }

  d->totalBits++;
  return d->leafs == 0;
}

static Bitmap *compReader_read_header_bitmap(CompReader *reader) {
  size_t minBits = compReader_count_header(reader);

  if (!minBits) {
    return NULL;
  }

  Bitmap *bm = bitmapInit(minBits);

  fs_do_next_bits(reader->fs, minBits, (void *)bm,
                  compReader_read_header_bitmap_callback);

  return bm;
}

static int compReader_read_header_bitmap_callback(int bit, void *data) {
  Bitmap *bm = data;
  bitmapAppendLeastSignificantBit(bm, bit);
  return 0;
}

static int compReader_translate_callback(int bit, void *data) {
  CallbackData2 *d = data;

  d->tree = huffmanTree_get_child(d->tree, bit);

  if (huffmanTree_is_leaf(d->tree)) {
    binWriter_write_byte(d->writer, huffmanTree_get_ASCII(d->tree));
    d->tree = d->root;
  }

  d->output.sizeBits++;

  return 0;
}

static unsigned long get_null_code_length(HuffmanTree *root) {
  HuffmanTree *nullhf = huffmanTree_search_ASCII(root, '\0');
  TreeCode code = huffmanTree_get_code(nullhf);
  return code.len;
}
