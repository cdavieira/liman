#pragma once

#include <stdio.h>

char* file_rem_path(char* filename);
char* file_add_pref(char* filename, char* pref);
char* file_rem_ext(char* filename);
char* file_add_ext(char* filename, char* ext);
ssize_t file_get_nbytes(FILE* fp);
ssize_t file_get_remaining_nbytes(FILE* fp);
