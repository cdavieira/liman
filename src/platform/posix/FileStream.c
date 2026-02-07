#include "platform/posix/FileStream.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/posix/file.h"
#include "platform/process.h"
#include "utils/bits.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

struct FileStream {
  int fd;

  unsigned char *buffer;
  size_t buffer_size;

  unsigned char *buffer_ptr;
  size_t buffer_ptr_offset;

  size_t total_blocks;
  size_t total_bytes;

  size_t current_byte_idx;
  size_t blocks_read;
};

static int fs_next_block(FileStream *fs);
static int fs_loop_init(FileStream *fs, const char *filename, void *data,
                        int (*callback)(int byte, void *data));
static void fs_set_initial_state(FileStream *fs);

FileStream *fs_new(size_t blocksize) {
  FileStream *fs = mem_zalloc(sizeof(struct FileStream));

  fs_set_initial_state(fs);

  fs->buffer_size = blocksize;
  fs->buffer = mem_zalloc(blocksize);

  return fs;
}

FileStream *fs_destroy(FileStream *fs) {
  if (fs_is_open(fs)) {
    fs_close_file(fs);
  }
  fs->buffer = mem_free(fs->buffer);
  return mem_free(fs);
}

int fs_open_file(FileStream *fs, const char *filename) {
  if (!filename) {
    return -1;
  }

  fs_set_initial_state(fs);

  fs->fd = open(filename, O_RDONLY);
  if (fs->fd == -1) {
    log_error("FileStream failed opening file: %s", filename);
    process_quick_abort();
  }

  fs->total_bytes = file_get_total_size_in_bytes_fd(fs->fd);
  fs->total_blocks = (fs->total_bytes / fs->buffer_size) +
                     (fs->total_bytes % fs->buffer_size == 0 ? 0 : 1);

  return fs_next_block(fs);
}

int fs_close_file(FileStream *fs) {
  int status = close(fs->fd);

  fs_set_initial_state(fs);

  return status;
}

int fs_rewind(FileStream *fs) {
  if (!fs_is_open(fs)) {
    return -1;
  }

  if (lseek(fs->fd, 0, SEEK_SET) == -1) {
    return -1;
  }

  memset(fs->buffer, 0, fs->buffer_size);
  fs->current_byte_idx = 0;
  fs->blocks_read = 0;

  return fs_next_block(fs);
}

int fs_is_open(FileStream *fs) { return fs && fs->fd != -1; }

int fs_next_byte(FileStream *fs) {
  size_t next_ptr_offset = fs->buffer_ptr_offset + 1;
  int out_of_buffer_boundary = next_ptr_offset >= fs->buffer_size;

  if (out_of_buffer_boundary) {
    if (fs_next_block(fs) <= 0) {
      return -1;
    }
  } else {
    fs->buffer_ptr++;
    fs->buffer_ptr_offset++;
  }

  fs->current_byte_idx++;
  return 0;
}

int fs_get_byte(FileStream *fs) {
  return fs->buffer_ptr ? *fs->buffer_ptr : EOF;
}

int fs_get_bit(FileStream *fs, int idx) {
  return bits_bitAt(*fs->buffer_ptr, idx);
}

size_t fs_total_blocks(FileStream *fs) { return fs->total_blocks; }

size_t fs_total_bytes(FileStream *fs) { return fs->total_bytes; }

size_t fs_remaining_bytes(FileStream *fs) {
  return fs->total_bytes - fs->current_byte_idx;
}

void fs_do_next_bytes(FileStream *fs, size_t count, void *data,
                      int (*callback)(int byte, void *data)) {
  for (size_t i = 0; i < count; i++) {
    int byte = fs_get_byte(fs);
    if (callback(byte, data)) {
      break;
    }

    if (fs_next_byte(fs) < 0) {
      break;
    }
  }
}

void fs_do_next_bits(FileStream *fs, size_t count, void *data,
                     int (*callback)(int bit, void *data)) {
  const size_t byte_count = bits_get_minimum_amount_of_required_bytes(count);
  for (size_t i = 0; i < byte_count; i++) {
    for (int i = 7; i >= 0; i--) {
      int bit = fs_get_bit(fs, i);
      if (callback(bit, data)) {
        return;
      }
    }

    if (fs_next_byte(fs) < 0) {
      break;
    }
  }
}

void fs_loop_over_all_bytes(FileStream *fs, const char *filename, void *data,
                            int (*callback)(int byte, void *data)) {
  if (fs_loop_init(fs, filename, data, callback) == -1) {
    return;
  }

  while (fs->current_byte_idx < fs->total_bytes) {
    int byte = fs_get_byte(fs);

    if (callback(byte, data)) {
      return;
    }

    if (fs_next_byte(fs) < 0) {
      return;
    }
  }

  fs_close_file(fs);
}

void fs_loop_over_all_bits(FileStream *fs, const char *filename, void *data,
                           int (*callback)(int bit, void *data)) {
  if (fs_loop_init(fs, filename, data, callback) == -1) {
    return;
  }

  while (fs->current_byte_idx < fs->total_bytes) {
    for (int i = 7; i >= 0; i--) {
      int bit = fs_get_bit(fs, i);
      if (callback(bit, data)) {
        return;
      }
    }

    if (fs_next_byte(fs) < 0) {
      return;
    }
  }

  fs_close_file(fs);
}

static int fs_next_block(FileStream *fs) {
  if (!fs_is_open(fs)) {
    return -1;
  }

  ssize_t bytes_read = read(fs->fd, fs->buffer, fs->buffer_size);

  fs->buffer_ptr = fs->buffer;
  fs->buffer_ptr_offset = 0;

  fs->blocks_read++;

  return bytes_read;
}

static int fs_loop_init(FileStream *fs, const char *filename, void *data,
                        int (*callback)(int byte, void *data)) {
  if (!fs || !callback) {
    return -1;
  }

  if (!fs_is_open(fs) && filename) {
    if (fs_open_file(fs, filename) <= 0) {
      return -1;
    }
  }

  return 0;
}

static void fs_set_initial_state(FileStream *fs) {
  fs->fd = -1;

  // fs->block_buffer = NULL;
  // fs->block_size = 0;

  fs->buffer_ptr = NULL;
  fs->buffer_ptr_offset = 0;

  fs->total_blocks = 0;
  fs->total_bytes = 0;

  fs->current_byte_idx = 0;
  fs->blocks_read = 0;
}

void fs_debug_internal_state(FileStream *fs) {
  log_debug("fd: %d\n", fs->fd);
  log_debug("buffer: %p (%zu)\n", fs->buffer, fs->buffer_size);
  log_debug("buffer_ptr: %p(+%zu)\n", fs->buffer_ptr, fs->buffer_ptr_offset);
  log_debug("bytes: %zu/%zu\n", fs->current_byte_idx, fs->total_bytes);
  log_debug("blocks: %zu/%zu\n", fs->blocks_read, fs->total_blocks);
}
