#include "platform/posix/file.h"
#include "platform/process.h"

#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

ssize_t file_get_total_size_in_bytes(FILE *fp) {
  long curpos = ftell(fp);

  fseek(fp, 0, SEEK_SET);
  long inipos = ftell(fp);

  fseek(fp, 0, SEEK_END);
  long endpos = ftell(fp);

  fseek(fp, curpos, SEEK_SET);

  return endpos - inipos;
}

ssize_t file_get_remaining_size_in_bytes(FILE *fp) {
  long curpos = ftell(fp);

  fseek(fp, 0, SEEK_END);
  long endpos = ftell(fp);

  fseek(fp, curpos, SEEK_SET);

  return endpos - curpos;
}

ssize_t file_get_total_size_in_bytes_fd(int fd) {
  struct stat st;

  if (fstat(fd, &st) == -1) {
    process_abort("fstat failed when reading fd");
  }

  return st.st_size;
}

ssize_t file_get_remaining_size_in_bytes_fd(int fd) {
  todo();
  return -1;
}

ssize_t file_get_total_size_in_bytes_filename(const char *filename) {
  int fd = open(filename, O_RDONLY);
  const ssize_t sz = file_get_total_size_in_bytes_fd(fd);
  close(fd);
  return sz;
}

ssize_t file_get_remaining_size_in_filename(const char *filename) {
  int fd = open(filename, O_RDONLY);
  const ssize_t sz = file_get_remaining_size_in_bytes_fd(fd);
  close(fd);
  return sz;
}
