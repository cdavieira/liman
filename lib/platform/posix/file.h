#pragma once

#include "utils/types/common.h"

ssize_t file_get_total_size_in_bytes(FILE *fp);
ssize_t file_get_remaining_size_in_bytes(FILE *fp);

ssize_t file_get_total_size_in_bytes_fd(int fd);
ssize_t file_get_remaining_size_in_bytes_fd(int fd);

ssize_t file_get_total_size_in_bytes_filename(const char *filename);
ssize_t file_get_remaining_size_in_filename(const char *filename);

char *file_find_basename(char *s);
char *file_find_extension(char *s);
int file_is_abspath_like(const char *s);
int file_is_relpath_like(const char *s);
const char *file_get_directory_delimiter(void);
const char *file_get_extension_delimiter(void);

enum PathType {
  PATH_ABSOLUTE,
  PATH_RELATIVE,
  PATH_FILENAME,
  PATH_INVALID,
};
enum PathType file_infer_type(const char *filename);
