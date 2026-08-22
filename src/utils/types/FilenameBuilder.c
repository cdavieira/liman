#include "utils/types/FilenameBuilder.h"
#include "platform/abort.h"
#include "platform/mem.h"
#include "utils/cstr.h"
#include "utils/types/String.h"
#include <string.h> //strlen, strrchr

// TODO: this struct name is pretty bad, it should be something like
// 'FilenameExtractor'

struct FilenameBuilder {
  char *copy;
  const char *path;
  const char *basename;
  const char *extension;
};

static void filenameBuilder_assert(const FilenameBuilder *builder) {
  if (builder->basename == NULL) {
    abort_default("At least a basename is required when building a filename.");
  }
}

static char *filenameBuilder_assemble(const char *path, const char *basename,
                                      const char *extension, const char *delim,
                                      const char *dot) {
  String *s = string_new();

  if (path) {
    string_append(s, path);
    string_append(s, delim);
  }

  string_append(s, basename);

  if (extension) {
    string_append(s, dot);
    string_append(s, extension);
  }

  return string_drain(s);
}

static enum PathType filenameBuilder_infer_type(const char *filename) {
  if (!filename) {
    return PATH_INVALID;
  }

  size_t sz = strlen(filename);
  if (!sz) {
    return PATH_INVALID;
  }

  // the following doesn't work for windows
  if (filename[0] == '/') {
    return PATH_ABSOLUTE;
  }

  if (cstr_find_after_last_char(filename, '/')) {
    return PATH_RELATIVE;
  }

  return PATH_FILENAME;
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
  char *basename = (char *)cstr_find_after_last_char(copy, '/');
  char *extension = (char *)cstr_find_at_last_char(copy, '.');

  if (extension) {
    *extension = '\0';
    extension++;
  }

  builder->copy = copy;
  if (basename) {
    builder->basename = basename;
    basename--;
    *basename = '\0';
    builder->path = copy;
  } else {
    builder->basename = copy;
  }
  builder->extension = extension;

  return builder;
}

FilenameParts filenameBuilder_drain(FilenameBuilder *builder) {
  FilenameParts parts = {
      .pathType = filenameBuilder_infer_type(builder->copy),
      .filename = builder->basename ? mem_salloc(builder->basename) : NULL,
      .extension = builder->extension ? mem_salloc(builder->extension) : NULL,
      .path = builder->path ? mem_salloc(builder->path) : NULL,
  };
  filenameBuilder_destroy(builder);
  return parts;
}

FilenameBuilder *filenameBuilder_from_parts(FilenameParts parts) {
  char *copy = filenameBuilder_assemble(parts.path, parts.filename,
                                        parts.extension, "/", ".");
  FilenameBuilder *b = filenameBuilder_from_filename(copy);
  copy = mem_free(copy);
  return b;
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

  char *filename = filenameBuilder_assemble(builder->path, builder->basename,
                                            builder->extension, "/", ".");

  return filename;
}
