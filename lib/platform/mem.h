#pragma once

#include "utils/types/common.h"

/**
 * allocate a chunk of data that is uninitialized
 * */
void *mem_alloc(size_t total_size);

/**
 * allocate a chunk of data that is initialized with zeros
 * */
void *mem_zalloc(size_t total_size);

/**
 * allocate a copy of a string
 * */
char *mem_salloc(const char *str);

/**
 * reallocate a chunk of data that is initialized with zeros
 * */
void *mem_realloc(void *ptr, size_t total_size);

/**
 * free any pointer previously allocated by a mem_* function
 * */
void *mem_free(void *ptr);
