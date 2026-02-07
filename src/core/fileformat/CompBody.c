#include "core/fileformat/CompBody.h"
#include "core/HuffmanTree.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/posix/FileStream.h"
#include "platform/posix/file.h"
#include "platform/process.h"
#include "utils/bits.h"
#include "utils/container/Bitmap.h"

#include <stdio.h>

typedef struct CompBodyMetadata {
  size_t compressed_min_size_in_bits;
  size_t compressed_pad_bits;
  size_t compressed_total_size_in_bits; // msgTotalBits % 8 == 0
} CompBodyMetadata;

struct CompBody {
  HuffmanTree *hf;
  Bitmap *bm;
  CompBodyMetadata metadata;
};

typedef struct {
  const Bitmap *lookup[256];
  Bitmap *dest;
} CallbackData;

static CompBodyMetadata compBodyMetadata_from_huffmanTree(HuffmanTree *root);
static CompBodyMetadata compBodyMetadata_from_fp(FILE *fp);
static void compBody_build_bitmap_lookup(const CompBody *body,
                                         const Bitmap *lookup[static 256]);
static int compBody_encode_fileStream_handler(int byte, void *smth);

static void compBody_init_bitmap_from_huffmanTree(CompBody *body);
static void compBody_init_bitmap_from_fp(CompBody *body, FILE *fp);
static int compBody_decode_fp_handler(int bit, void *smth);

CompBody *compBody_from_huffmanTree(HuffmanTree *root) {
  CompBody *body = mem_alloc(sizeof(CompBody));
  body->hf = root;
  return body;
}

CompBody *compBody_destroy(CompBody *body) {
  bitmapLibera(body->bm);
  mem_free(body);
  return NULL;
}

size_t compBody_get_total_compressed_size_in_bits(CompBody *body) {
  return body->metadata.compressed_total_size_in_bits;
}

size_t compBody_get_total_compressed_size_in_bytes(CompBody *body) {
  return bits_toBytes(body->metadata.compressed_total_size_in_bits);
}

void compBody_encode_fileStream(CompBody *body, const char *filename) {
  const size_t buffersize = 1024 * 1024; // 1 MiB
  FileStream *fs = fs_new(buffersize);

  compBody_init_bitmap_from_huffmanTree(body);

  CallbackData *data = mem_alloc(sizeof(CallbackData));
  compBody_build_bitmap_lookup(body, data->lookup);
  data->dest = body->bm;

  fs_loop_over_all_bytes(fs, filename, (void *)data,
                         compBody_encode_fileStream_handler);
  bitmapConcat(body->bm, data->lookup[0]);

  fs_destroy(fs);

  mem_free(data);
}

void compBody_dump_into_fp(CompBody *body, FILE *fp) {
  fputc((unsigned char)body->metadata.compressed_pad_bits, fp);
  bitmapDump(body->bm, fp);
}

static CompBodyMetadata compBodyMetadata_from_huffmanTree(HuffmanTree *root) {
  CompBodyMetadata metadata;

  metadata.compressed_min_size_in_bits = huffmanTree_get_msg_size(root);
  metadata.compressed_pad_bits =
      bits_padding(metadata.compressed_min_size_in_bits);
  metadata.compressed_total_size_in_bits =
      metadata.compressed_min_size_in_bits + metadata.compressed_pad_bits;

  return metadata;
}

static CompBodyMetadata compBodyMetadata_from_fp(FILE *fp) {
  char padBits = fgetc(fp);
  if ((padBits < 0) || padBits > 7) {
    process_abort("Body pad field out of range");
  }

  CompBodyMetadata metadata;

  metadata.compressed_total_size_in_bits =
      file_get_remaining_size_in_bytes(fp) * 8;
  metadata.compressed_pad_bits = padBits;
  metadata.compressed_min_size_in_bits =
      metadata.compressed_total_size_in_bits - metadata.compressed_pad_bits;

  return metadata;
}

static void compBody_build_bitmap_lookup(const CompBody *body,
                                         const Bitmap *lookup[static 256]) {
  HuffmanTree *t = NULL;
  HuffmanTree *root = body->hf;
  for (unsigned i = 0; i < 256; i++) {
    t = huffmanTree_search_ASCII(root, i);
    lookup[i] = t ? huffmanTree_get_bitmap(t) : NULL;
  }
}

static int compBody_encode_fileStream_handler(int byte, void *smth) {
  CallbackData *data = (CallbackData *)smth;

  const Bitmap *bmapCode = data->lookup[byte];

#ifdef DEBUG
  log_debug("%d -> ", byte);
  bitmapPrint(bmapCode, stdout);
  putchar('\n');
#endif

  // TODO: storing the whole file as a Bitmap seems like a waste of memory and a
  // potential security vulnerability
  if (bmapCode) {
    bitmapConcat(data->dest, bmapCode);
  }

  return 0;
}

static void compBody_init_bitmap_from_huffmanTree(CompBody *body) {
  body->metadata = compBodyMetadata_from_huffmanTree(body->hf);
  body->bm = bitmapInit(body->metadata.compressed_min_size_in_bits);
}

static void compBody_init_bitmap_from_fp(CompBody *body, FILE *fp) {
  body->metadata = compBodyMetadata_from_fp(fp);
  body->bm = bitmapInit(body->metadata.compressed_min_size_in_bits);
}

static int compBody_decode_fp_handler(int bit, void *smth) {
  Bitmap *bm = smth;
  bitmapAppendLeastSignificantBit(bm, bit);
  return 0;
}
