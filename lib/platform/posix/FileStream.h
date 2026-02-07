#pragma once

#include "utils/types/common.h"

/**
 * A buffered file stream reader
 * */
typedef struct FileStream FileStream;

/**
 * Create a new file stream, which buffers <buffersize> bytes per read
 * */
FileStream *fs_new(size_t buffersize);
FileStream *fs_destroy(FileStream *fs);

int fs_open_file(FileStream *fs, const char *filename);
int fs_close_file(FileStream *fs);

/**
 * Restart the file stream to the beginning of the file
 * */
int fs_rewind(FileStream *fs);

int fs_is_open(FileStream *fs);

int fs_next_byte(FileStream *fs);

int fs_get_byte(FileStream *fs);
int fs_get_bit(FileStream *fs, int idx);

size_t fs_total_blocks(FileStream *fs);
size_t fs_total_bytes(FileStream *fs);
size_t fs_remaining_bytes(FileStream *fs);

/** iterate the next 'count' bytes of the file. Calling this function always
 * starts at the next available byte in the file.
 */
void fs_do_next_bytes(FileStream *fs, size_t count, void *data,
                      int (*callback)(int byte, void *data));

/** iterate the next 'count' bits of the file. Calling this function always
 * starts at the next available byte in the file.
 */
void fs_do_next_bits(FileStream *fs, size_t count, void *data,
                     int (*callback)(int bit, void *data));

/** iterate over all bytes of 'filename', except for the EOF byte
 */
void fs_loop_over_all_bytes(FileStream *fs, const char *filename, void *data,
                            int (*callback)(int byte, void *data));

/** iterate over all bytes of 'filename', except for the EOF byte
 */
void fs_loop_over_all_bits(FileStream *fs, const char *filename, void *data,
                           int (*callback)(int bit, void *data));

void fs_debug_internal_state(FileStream *fs);
