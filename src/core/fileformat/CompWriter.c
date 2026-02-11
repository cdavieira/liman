#include "core/fileformat/CompWriter.h"
#include "core/fileformat/CompBody.h"
#include "core/fileformat/CompHeader.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/posix/BinaryWriter.h"
#include "platform/posix/file.h"
#include "platform/process.h"
#include "utils/bits.h"
#include <stdio.h>

struct CompWriter {
  CompHeader *header;
  CompBody *body;
  const char *inputfile;
};

CompWriter *compWriter_from_huffmanTree(HuffmanTree *ht) {
  CompWriter *writer = mem_alloc(sizeof(struct CompWriter));
  writer->header = compHeader_from_huffmanTree(ht);
  writer->body = compBody_from_huffmanTree(ht);
  writer->inputfile = NULL;
  return writer;
}

CompWriter *compWriter_destroy(CompWriter *writer) {
  writer->header = compHeader_destroy(writer->header);
  writer->body = compBody_destroy(writer->body);
  writer = mem_free(writer);
  return NULL;
}

void compWriter_set_inputfile(CompWriter *writer, const char *inputfile) {
  if (writer->inputfile == NULL) {
    writer->inputfile = inputfile;
  }
}

CompWriterStatistics compWriter_get_statistics(CompWriter *writer) {
  size_t oldsz = compWriter_get_total_uncompressed_size_in_bytes(writer);
  size_t newsz = compWriter_get_total_compressed_size_in_bytes(writer);
  float compression_rate = ((double)oldsz - (double)newsz) / oldsz;
  return (CompWriterStatistics){
      .compression_rate = compression_rate,
      .compressed_size = newsz,
      .uncompressed_size = oldsz,
  };
}

size_t compWriter_get_total_compressed_size_in_bytes(CompWriter *writer) {
  size_t header_size = compHeader_get_total_size_in_bits(writer->header);
  size_t padding_size = 8; // bits
  size_t body_size = compBody_get_total_compressed_size_in_bits(writer->body);
  return bits_toBytes(header_size + padding_size + body_size);
}

size_t compWriter_get_total_uncompressed_size_in_bytes(CompWriter *writer) {
  if (writer->inputfile == NULL) {
    return 0;
  }
  return file_get_total_size_in_bytes_filename(writer->inputfile);
}

float compWriter_get_compression_rate(CompWriter *writer) {
  return compWriter_get_statistics(writer).compression_rate;
}

void compWriter_dump(CompWriter *writer, const char *outputfile) {
  BinaryWriter *binWriter = binWriter_new(4096 * 8); // 4 KiB
  binWriter_open(binWriter, outputfile);

  // Writing the header
  compHeader_dump_into_binaryWriter(writer->header, binWriter);
  // force writing the header with byte alignment to output file
  // this guarantees that the header will be written with a size in bits that is
  // a multiple of 8
  binWriter_flush(binWriter);

  // Writing the padding + body
  compBody_encode(writer->body, writer->inputfile, binWriter);

  binWriter_destroy(binWriter);
}
