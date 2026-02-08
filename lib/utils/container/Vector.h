#pragma once

#include "utils/types/common.h"

typedef struct Vector Vector;

Vector *vector_new(size_t min);
Vector *vector_destroy(Vector *arr, void *(*destroy_element)(void *));

size_t vector_get_size(const Vector *arr);
size_t vector_get_capacity(const Vector *arr);
void *vector_get_item(Vector *arr, size_t idx);

void *vector_search(Vector *arr, void *item, int (*compare)(void *, void *));
size_t vector_append(Vector *arr, void *item);
void *vector_pop(Vector *arr);

int vector_is_empty(const Vector *arr);
int vector_is_full(const Vector *arr);
