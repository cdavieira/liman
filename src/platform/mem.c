#include "platform/mem.h"

#include <string.h>

#include <stdlib.h>

void *mem_alloc(size_t total_size) { return malloc(total_size); }

void *mem_zalloc(size_t total_size) { return calloc(1, total_size); }

char *mem_salloc(const char *str) { return strdup(str); }

void *mem_realloc(void *ptr, size_t total_size) {
  return realloc(ptr, total_size);
}

void *mem_free(void *ptr) {
  if (ptr) {
    free(ptr);
  }
  return NULL;
}
