#include "platform/posix/BinaryWriter.h"
#include "core/HuffmanCode.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "utils/bits.h"
#include "utils/container/Bitmap.h"

#include <fcntl.h>
#include <unistd.h>

struct BinaryWriter {
  int fd;
  const char *filename;

  size_t block_size;
  size_t bits_written;
  Bitmap *buffer;
};

static inline size_t binWriter_remaining_bits_until_dump(BinaryWriter *writer) {
  return writer->block_size > writer->bits_written
             ? writer->block_size - writer->bits_written
             : 0;
}

static inline int binWriter_should_write(BinaryWriter *writer) {
  return writer->bits_written >= writer->block_size;
}

static inline void binWriter_cleanup_buffer(BinaryWriter *writer) {
  bitmapLibera(writer->buffer);
  writer->buffer = bitmapInit(writer->block_size);
  writer->bits_written = 0;
}

BinaryWriter *binWriter_new(size_t block_size) {
  BinaryWriter *writer = mem_alloc(sizeof(struct BinaryWriter));
  writer->fd = -1;
  writer->block_size = block_size;
  writer->bits_written = 0;
  writer->filename = NULL;
  writer->buffer = bitmapInit(writer->block_size);
  return writer;
}

BinaryWriter *binWriter_destroy(BinaryWriter *writer) {
  binWriter_close(writer);
  bitmapLibera(writer->buffer);
  return mem_free(writer);
}

void binWriter_open(BinaryWriter *writer, const char *filename) {
  if (binWriter_is_open(writer)) {
    return;
  }
  writer->filename = filename;
  const int permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  writer->fd = open(writer->filename, O_WRONLY | O_CREAT, permissions);
}

void binWriter_close(BinaryWriter *writer) {
  if (!binWriter_is_open(writer)) {
    return;
  }
  binWriter_flush(writer);
  close(writer->fd);
  writer->fd = -1;
}

int binWriter_is_open(BinaryWriter *writer) { return writer->fd != -1; }

void binWriter_flush(BinaryWriter *writer) {
  size_t size = bitmapGetLength(writer->buffer);
  if (size == 0) {
    log_warning("binWriter: attempting to write 0 bytes to %s",
                writer->filename);
    return;
  }

  size_t padding = bits_padding(size);
  if (padding) {
    log_warning(
        "binWriter: about to add some padding to file %s: %zu bits to fill %zu",
        writer->filename, padding, size);
  }

  size_t required_bytes = bits_get_minimum_amount_of_required_bytes(size);
  unsigned char *buffer = bitmapGetContents(writer->buffer);
  ssize_t bytes_written = write(writer->fd, buffer, required_bytes);

  if (required_bytes != bytes_written) {
    log_warning("binWriter: bytes attempted/written mismatch");
  }

  binWriter_cleanup_buffer(writer);
}

void binWriter_write_bit(BinaryWriter *writer, int bit) {
  if (binWriter_should_write(writer)) {
    binWriter_flush(writer);
  }
  bitmapAppendLeastSignificantBit(writer->buffer, bit);
  writer->bits_written++;
}

void binWriter_write_byte(BinaryWriter *writer, unsigned char byte) {
  size_t sz = 8;
  size_t remaining_bits = binWriter_remaining_bits_until_dump(writer);
  if (sz <= remaining_bits) {
    for (int i = sz - 1; i >= 0; i--) {
      bitmapAppendLeastSignificantBit(writer->buffer,
                                      bits_get_char_bit(byte, i));
    }
    writer->bits_written += sz;
    return;
  }

  size_t exceeding_bits = sz - remaining_bits;
  int i = sz - 1;
  for (; i >= exceeding_bits; i--) {
    bitmapAppendLeastSignificantBit(writer->buffer, bits_get_size_bit(byte, i));
  }
  writer->bits_written += remaining_bits;
  binWriter_flush(writer);
  for (; i >= 0; i--) {
    bitmapAppendLeastSignificantBit(writer->buffer, bits_get_size_bit(byte, i));
  }
  writer->bits_written += exceeding_bits;
}

void binWriter_write_bitmap(BinaryWriter *writer, const Bitmap *bm) {
  size_t sz = bitmapGetLength(bm);
  size_t remaining_bits = binWriter_remaining_bits_until_dump(writer);
  if (sz <= remaining_bits) {
    bitmapConcat(writer->buffer, bm);
    writer->bits_written += sz;
    return;
  }

  size_t exceeding_bits = sz - remaining_bits;
  for (size_t i = 0; i < remaining_bits; i++) {
    bitmapAppendLeastSignificantBit(writer->buffer, bitmapGetBit(bm, i));
  }
  writer->bits_written += remaining_bits;
  binWriter_flush(writer);
  for (size_t i = remaining_bits; i < sz; i++) {
    bitmapAppendLeastSignificantBit(writer->buffer, bitmapGetBit(bm, i));
  }
  writer->bits_written += exceeding_bits;
}

void binWriter_write_huffmanCode(BinaryWriter *writer,
                                 const HuffmanCode *hfcode) {
  Code code = huffmanCode_get_value(hfcode);
  size_t sz = code.len;
  if (!sz) {
    return;
  }

  size_t remaining_bits = binWriter_remaining_bits_until_dump(writer);
  if (sz <= remaining_bits) {
    for (int i = sz - 1; i >= 0; i--) {
      bitmapAppendLeastSignificantBit(writer->buffer,
                                      bits_get_size_bit(code.code, i));
    }
    writer->bits_written += sz;
    return;
  }

  size_t exceeding_bits = sz - remaining_bits;
  int i = sz - 1;
  for (; i >= exceeding_bits; i--) {
    bitmapAppendLeastSignificantBit(writer->buffer,
                                    bits_get_size_bit(code.code, i));
  }
  writer->bits_written += remaining_bits;
  binWriter_flush(writer);
  for (; i >= 0; i--) {
    bitmapAppendLeastSignificantBit(writer->buffer,
                                    bits_get_size_bit(code.code, i));
  }
  writer->bits_written += exceeding_bits;
}
