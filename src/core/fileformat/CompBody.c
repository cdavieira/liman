#include "core/fileformat/CompBody.h"
#include "core/CodeLookup.h"
#include "core/HuffmanCode.h"
#include "core/HuffmanTree.h"
#include "platform/abort.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/posix/BinaryWriter.h"
#include "platform/posix/FileStream.h"
#include "platform/posix/file.h"
#include "utils/bits.h"

#include <stdio.h>

typedef struct CompBodyMetadata {
  size_t compressed_min_size_in_bits;
  size_t compressed_pad_bits;
  size_t compressed_total_size_in_bits; // msgTotalBits % 8 == 0
} CompBodyMetadata;

struct CompBody {
  HuffmanTree *hf;
  CompBodyMetadata metadata;
};

typedef struct CallbackData {
  CodeLookup *lookup;
  BinaryWriter *writer;
} CallbackData;

static CompBodyMetadata compBodyMetadata_from_huffmanTree(HuffmanTree *root);
// static CompBodyMetadata compBodyMetadata_from_fp(FILE *fp);
static int compBody_encode_handler(int byte, void *smth);

static void compBody_init_bitmap_from_huffmanTree(CompBody *body);

CompBody *
compBody_from_huffmanTree(HuffmanTree *root)
{
  CompBody *body = mem_alloc(sizeof(CompBody));
  body->hf = root;
  return body;
}

CompBody *
compBody_destroy(CompBody *body)
{
  mem_free(body);
  return NULL;
}

size_t
compBody_get_total_compressed_size_in_bits(CompBody *body)
{
  return body->metadata.compressed_total_size_in_bits;
}

void
compBody_encode(CompBody *body, const char *filename, BinaryWriter *writer)
{
  const size_t buffersize = 1024 * 1024; // 1 MiB
  FileStream *fs = fs_new(buffersize);

  compBody_init_bitmap_from_huffmanTree(body);

  CallbackData *data = mem_alloc(sizeof(CallbackData));
  data->lookup = codeLookup_from_huffmanTree(body->hf);
  data->writer = writer;

  binWriter_write_byte(writer,
                       (unsigned char)body->metadata.compressed_pad_bits);
  fs_loop_over_all_bytes(fs, filename, (void *)data, compBody_encode_handler);

  fs_destroy(fs);

  data->lookup = codeLookup_destroy(data->lookup);
  mem_free(data);
}

static CompBodyMetadata
compBodyMetadata_from_huffmanTree(HuffmanTree *root)
{
  CompBodyMetadata metadata;

  metadata.compressed_min_size_in_bits = huffmanTree_get_msg_size(root);
  metadata.compressed_pad_bits =
      bits_padding(metadata.compressed_min_size_in_bits);
  metadata.compressed_total_size_in_bits =
      metadata.compressed_min_size_in_bits + metadata.compressed_pad_bits;

  return metadata;
}

// static CompBodyMetadata
// compBodyMetadata_from_fp(FILE *fp)
// {
//   char padBits = fgetc(fp);
//   if ((padBits < 0) || padBits > 7)
//   {
//     abort_default("Body pad field out of range");
//   }
//
//   CompBodyMetadata metadata;
//
//   metadata.compressed_total_size_in_bits =
//       file_get_remaining_size_in_bytes(fp) * 8;
//   metadata.compressed_pad_bits = padBits;
//   metadata.compressed_min_size_in_bits =
//       metadata.compressed_total_size_in_bits - metadata.compressed_pad_bits;
//
//   return metadata;
// }

static int
compBody_encode_handler(int byte, void *smth)
{
  CallbackData *data = (CallbackData *)smth;

  const HuffmanCode *code = codeLookup_get(data->lookup, byte);

  if (huffmanCode_has_value(code))
  {
    binWriter_write_huffmanCode(data->writer, code);
  }

  return 0;
}

static void
compBody_init_bitmap_from_huffmanTree(CompBody *body)
{
  body->metadata = compBodyMetadata_from_huffmanTree(body->hf);
}
