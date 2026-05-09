#pragma once

typedef struct FilenameBuilder FilenameBuilder;

enum PathType {
  PATH_ABSOLUTE,
  PATH_RELATIVE,
  PATH_FILENAME,
  PATH_INVALID,
};

typedef struct FilenameParts {
  char *path;
  char *filename;
  char *extension;
  enum PathType pathType;
} FilenameParts;

FilenameBuilder *filenameBuilder_new(void);
FilenameBuilder *filenameBuilder_from_filename(const char *filename);
FilenameParts filenameBuilder_drain(FilenameBuilder *builder);
FilenameBuilder *filenameBuilder_from_parts(FilenameParts parts);
FilenameBuilder *filenameBuilder_destroy(FilenameBuilder *builder);

void filenameBuilder_set_basename(FilenameBuilder *builder,
                                  const char *basename);
void filenameBuilder_set_extension(FilenameBuilder *builder,
                                   const char *extension);
void filenameBuilder_set_path(FilenameBuilder *builder, const char *path);

void filenameBuilder_set_path_to_current_directory(FilenameBuilder *builder);
void filenameBuilder_remove_extension(FilenameBuilder *builder);

char *filenameBuilder_build(const FilenameBuilder *builder);
