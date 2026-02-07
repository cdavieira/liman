#pragma once

#include "utils/types/common.h"

ssize_t file_get_total_size_in_bytes(FILE *fp);
ssize_t file_get_remaining_size_in_bytes(FILE *fp);

ssize_t file_get_total_size_in_bytes_fd(int fd);
ssize_t file_get_remaining_size_in_bytes_fd(int fd);

ssize_t file_get_total_size_in_bytes_filename(const char *filename);
ssize_t file_get_remaining_size_in_filename(const char *filename);
