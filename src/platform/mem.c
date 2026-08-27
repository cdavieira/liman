#include "platform/mem.h"

#include <string.h>

#include <stdlib.h>

void *
mem_alloc(size_t total_size)
{
  void *ptr = malloc(total_size);
  return ptr;
}

void *
mem_zalloc(size_t total_size)
{
  void *ptr = calloc(1, total_size);
  return ptr;
}

char *
mem_salloc(const char *str)
{
  char *cpy = strdup(str);
  return cpy;
}

void *
mem_realloc(void *ptr, size_t total_size)
{
  void *cpy = realloc(ptr, total_size);
  return cpy;
}

void *
mem_free(void *ptr)
{
  if (ptr)
  {
    free(ptr);
  }
  return NULL;
}
