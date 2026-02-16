#include "app/filenaming.h"
#include "platform/mem.h"
#include "utils/cstr.h"
#include "utils/types/FilenameBuilder.h"
#include "utils/types/String.h"

/**
 * this implementation sucks, but it suffices for now
 *
 * it will be improved over time (hopefully) */

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

char *liman_get_uncompressed_filename(const char *inputfile) {
  char *outputfile = NULL;
  const char *prefix = "unhuffman-";
  const char *basename = cstr_find_after_last_char(inputfile, '/');
  String *s = string_from_ptr(basename ? basename : inputfile);

  string_shift(s, prefix);
  outputfile = string_drain(s);

  return outputfile;
}

char *liman_get_codes_filename(const char *inputfile) {
  char *outputfile = NULL;
  const char *extension = ".txt";
  const char *prefix = "huhman-";
  String *s = string_from_ptr(inputfile);

  string_shift(s, prefix);
  string_append(s, extension);
  outputfile = string_drain(s);

  return outputfile;
}

char *liman_get_tree_filename(const char *inputfile) {
  char *outputfile = NULL;
  const char *extension = ".dot";
  const char *prefix = "huhman-";
  String *s = string_from_ptr(inputfile);

  string_shift(s, prefix);
  string_append(s, extension);
  outputfile = string_drain(s);

  return outputfile;
}

char *liman_build_compressed_filename(const char *inputfile) {
  char *filename = NULL;

  FilenameBuilder *b = filenameBuilder_from_filename(inputfile);

  filenameBuilder_set_path_to_current_directory(b);
  filenameBuilder_set_basename(b, inputfile);
  filenameBuilder_set_extension(b, "comp");

  filename = filenameBuilder_build(b);

  filenameBuilder_destroy(b);

  return filename;
}
