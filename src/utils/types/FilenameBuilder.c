#include "utils/types/FilenameBuilder.h"
#include "platform/mem.h"
#include "platform/process.h"
#include "utils/cstr.h"
#include "utils/types/String.h"
#include <string.h> //strlen, strrchr

struct FilenameBuilder {
  char *copy;
  const char *path;
  const char *basename;
  const char *extension;
};

static void filenameBuilder_assert(const FilenameBuilder *builder) {
  if (builder->basename == NULL) {
    process_abort("At least a basename is required when building a filename.");
  }
}

FilenameBuilder *filenameBuilder_new(void) {
  FilenameBuilder *builder = mem_alloc(sizeof(struct FilenameBuilder));
  builder->copy = NULL;
  builder->path = NULL;
  builder->basename = NULL;
  builder->extension = NULL;
  return builder;
}

FilenameBuilder *filenameBuilder_from_filename(const char *filename) {
  FilenameBuilder *builder = filenameBuilder_new();

  char *copy = mem_salloc(filename);
  const char *basename = cstr_find_after_last_char(copy, '/');
  char *extension = cstr_find_at_last_char(copy, '.');

  if (extension) {
    *extension = '\0';
    extension++;
  }

  builder->copy = copy;
  builder->basename = basename ? basename : filename;
  builder->extension = extension;

  return builder;
}

FilenameBuilder *filenameBuilder_destroy(FilenameBuilder *builder) {
  builder->copy = mem_free(builder->copy);
  return mem_free(builder);
}

void filenameBuilder_set_basename(FilenameBuilder *builder,
                                  const char *basename) {
  builder->basename = cstr_find_after_last_char(basename, '/');
}

void filenameBuilder_set_extension(FilenameBuilder *builder,
                                   const char *extension) {
  builder->extension = extension;
}

void filenameBuilder_set_path(FilenameBuilder *builder, const char *path) {
  builder->path = path;
}

void filenameBuilder_set_path_to_current_directory(FilenameBuilder *builder) {
  filenameBuilder_set_path(builder, ".");
}

void filenameBuilder_remove_extension(FilenameBuilder *builder) {
  filenameBuilder_set_extension(builder, NULL);
}

char *filenameBuilder_build(const FilenameBuilder *builder) {
  filenameBuilder_assert(builder);

  char *filename = NULL;
  const char *delim = "/";
  const char *dot = ".";
  const char *ptrs[] = {
      builder->path, delim, builder->basename, dot, builder->extension,
  };
  String *s = string_empty_from_ptrs(5, ptrs);

  if (builder->path) {
    string_append(s, builder->path);
    string_append(s, delim);
  }

  string_append(s, builder->basename);

  if (builder->extension) {
    string_append(s, dot);
    string_append(s, builder->extension);
  }

  filename = string_drain(s);

  return filename;
}
