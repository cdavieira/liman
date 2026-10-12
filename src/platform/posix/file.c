#include "platform/posix/file.h"
#include "platform/abort.h"
#include "utils/cstr.h"

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

ssize_t
file_get_total_size_in_bytes(FILE *fp)
{
  long curpos = ftell(fp);

  fseek(fp, 0, SEEK_SET);
  long inipos = ftell(fp);

  fseek(fp, 0, SEEK_END);
  long endpos = ftell(fp);

  fseek(fp, curpos, SEEK_SET);

  return endpos - inipos;
}

ssize_t
file_get_remaining_size_in_bytes(FILE *fp)
{
  long curpos = ftell(fp);

  fseek(fp, 0, SEEK_END);
  long endpos = ftell(fp);

  fseek(fp, curpos, SEEK_SET);

  return endpos - curpos;
}

ssize_t
file_get_total_size_in_bytes_fd(int fd)
{
  struct stat st;

  if (fstat(fd, &st) == -1)
  {
    abort_default("fstat failed when reading fd");
  }

  return st.st_size;
}

ssize_t
file_get_remaining_size_in_bytes_fd(int fd)
{
  todo();
  return -1;
}

ssize_t
file_get_total_size_in_bytes_filename(const char *filename)
{
  int fd = open(filename, O_RDONLY);
  const ssize_t sz = file_get_total_size_in_bytes_fd(fd);
  close(fd);
  return sz;
}

ssize_t
file_get_remaining_size_in_filename(const char *filename)
{
  int fd = open(filename, O_RDONLY);
  const ssize_t sz = file_get_remaining_size_in_bytes_fd(fd);
  close(fd);
  return sz;
}

static const char *SYMBOL_STR_DIR_CURRENT = ".";
static const char *SYMBOL_STR_DIR_DELIMITER = "/";
static const char *SYMBOL_STR_EXT_SEPARATOR = ".";
static const char *SYMBOL_STR_ROOTDIR = "/";
static const char SYMBOL_CHR_DIR_CURRENT = '.';
static const char SYMBOL_CHR_DIR_DELIMITER = '/';
static const char SYMBOL_CHR_EXT_SEPARATOR = '.';

char *
file_find_basename(char *s)
{
  return (char *)cstr_find_after_last_char(s, SYMBOL_CHR_DIR_DELIMITER);
}

char *
file_find_extension(char *s)
{
  return (char *)cstr_find_at_last_char(s, SYMBOL_CHR_EXT_SEPARATOR);
}

int
file_is_abspath_like(const char *s)
{
  return s[0] == SYMBOL_STR_ROOTDIR[0];
}

int
file_is_relpath_like(const char *s)
{
  return cstr_find_after_last_char(s, SYMBOL_CHR_DIR_DELIMITER) != NULL;
}

const char *
file_get_directory_delimiter(void)
{
  return SYMBOL_STR_DIR_DELIMITER;
}

const char *
file_get_extension_delimiter(void)
{
  return SYMBOL_STR_EXT_SEPARATOR;
}

enum PathType
file_infer_type(const char *filename)
{
  if (!filename)
  {
    return PATH_INVALID;
  }

  size_t sz = strlen(filename);
  if (!sz)
  {
    return PATH_INVALID;
  }

  if (file_is_abspath_like(filename))
  {
    return PATH_ABSOLUTE;
  }

  if (file_is_relpath_like(filename))
  {
    return PATH_RELATIVE;
  }

  return PATH_FILENAME;
}
