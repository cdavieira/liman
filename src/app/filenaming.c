#include "app/filenaming.h"
#include "platform/mem.h"
#include "utils/cstr.h"
#include "utils/types/FilenameBuilder.h"
#include "utils/types/String.h"

/**
 * this implementation sucks, but it suffices for now
 *
 * it will be improved over time (hopefully) */

static char *liman_get_uncompressed_filename(const char *inputfile);

static char *liman_get_inspect_filename(const char *inputfile,
                                        const char *prefix, char *extension);

char *liman_build_uncompressed_filename(const char *inputfile) {
  char *filename = NULL;

  char *outputfile = liman_get_uncompressed_filename(inputfile);
  FilenameBuilder *b = filenameBuilder_from_filename(outputfile);

  filenameBuilder_set_path_to_current_directory(b);
  filenameBuilder_remove_extension(b);

  filename = filenameBuilder_build(b);

  filenameBuilder_destroy(b);
  mem_free(outputfile);

  return filename;
}

char *liman_get_codes_filename(const char *inputfile) {
  return liman_get_inspect_filename(inputfile, "inspect-", "txt");
}

char *liman_get_tree_filename(const char *inputfile) {
  return liman_get_inspect_filename(inputfile, "inspect-", "dot");
}

char *liman_build_compressed_filename(const char *inputfile) {
  char *filename = NULL;

  FilenameBuilder *b = filenameBuilder_from_filename(inputfile);
  FilenameParts parts = filenameBuilder_drain(b);

  String *t = string_new();
  string_append(t, parts.extension);
  string_append(t, ".comp");
  char *ext = string_drain(t);

  b = filenameBuilder_from_parts(parts);

  filenameBuilder_set_path_to_current_directory(b);
  filenameBuilder_set_extension(b, ext);

  filename = filenameBuilder_build(b);

  filenameBuilder_destroy(b);
  parts.filename = mem_free(parts.filename);
  parts.extension = mem_free(parts.extension);
  parts.path = mem_free(parts.path);

  return filename;
}

/* Internals */

static char *liman_get_uncompressed_filename(const char *inputfile) {
  char *outputfile = NULL;
  const char *prefix = "unhuffman-";
  const char *basename = cstr_find_after_last_char(inputfile, '/');
  String *s = string_from_ptr(basename ? basename : inputfile);

  string_shift(s, prefix);
  outputfile = string_drain(s);

  return outputfile;
}

static char *liman_get_inspect_filename(const char *inputfile,
                                        const char *prefix, char *extension) {
  char *outputfile = NULL;

  FilenameBuilder *b = filenameBuilder_from_filename(inputfile);

  FilenameParts parts = filenameBuilder_drain(b);
  parts.extension = mem_free(parts.extension);
  parts.extension = extension;

  String *s = string_from_ptr(parts.filename);
  string_shift(s, prefix);
  parts.filename = mem_free(parts.filename);
  parts.filename = string_drain(s);

  b = filenameBuilder_from_parts(parts);

  outputfile = filenameBuilder_build(b);

  parts.filename = mem_free(parts.filename);
  parts.path = mem_free(parts.path);
  b = filenameBuilder_destroy(b);

  return outputfile;
}
