#pragma once

typedef struct FilenameBuilder FilenameBuilder;

FilenameBuilder *filenameBuilder_new(void);
FilenameBuilder *filenameBuilder_from_filename(const char *filename);
FilenameBuilder *filenameBuilder_destroy(FilenameBuilder *builder);

void filenameBuilder_set_basename(FilenameBuilder *builder,
                                  const char *basename);
void filenameBuilder_set_extension(FilenameBuilder *builder,
                                   const char *extension);
void filenameBuilder_set_path(FilenameBuilder *builder, const char *path);

void filenameBuilder_set_path_to_current_directory(FilenameBuilder *builder);
void filenameBuilder_remove_extension(FilenameBuilder *builder);

char *filenameBuilder_build(const FilenameBuilder *builder);
