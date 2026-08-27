#include "utils/container/Vector.h"
#include "platform/mem.h"
#include <assert.h> //assert
#include <stdio.h>  //printf

/*
 * this vector implementation has (always) at least a certain number
 * (MIN) of elements
 *
 * once the vector hits its limit, its capacity is doubled.
 *
 * if the vector size becomes oversized (size < 0.25*capacity), its
 * capacity is halfed */
struct Vector {
  void **elements;
  size_t size;
  size_t capacity;
  size_t min;
};

static void vector_grow(Vector *const arr, const size_t f);
static void vector_shrink(Vector *const arr, const size_t f);
// vector oversize: its current size is less than 25% of its current
// capacity
static int vector_is_oversized(const Vector *const arr);

Vector *
vector_new(size_t min)
{
  Vector *arr = mem_alloc(sizeof(Vector));
  arr->size = 0;
  arr->min = min;
  arr->capacity = min;
  arr->elements = mem_zalloc(arr->capacity * sizeof(void *));
  return arr;
}

Vector *
vector_destroy(Vector *arr, void *(*destroy_element)(void *))
{
  if (!arr)
  {
    return NULL;
  }
  if (destroy_element)
  {
    for (size_t i = 0; i < arr->size; i++)
    {
      arr->elements[i] = destroy_element(arr->elements[i]);
    }
  }
  mem_free(arr->elements);
  mem_free(arr);
  return NULL;
}

size_t
vector_get_size(const Vector *arr)
{
  return arr->size;
}

size_t
vector_get_capacity(const Vector *arr)
{
  return arr->capacity;
}

void *
vector_get_item(Vector *arr, size_t idx)
{
  return arr->elements[idx];
}

void *
vector_search(Vector *arr, void *item, int (*compare)(void *, void *))
{
  for (size_t i = 0; i < arr->size; i++)
  {
    if (compare(arr->elements[i], item))
    {
      return arr->elements[i];
    }
  }
  return NULL;
}

size_t
vector_append(Vector *arr, void *item)
{
  if (vector_is_full(arr))
  {
    vector_grow(arr, 1);
  }
  size_t idx = arr->size;
  arr->elements[idx] = item;
  arr->size++;
  return idx;
}

void *
vector_pop(Vector *arr)
{
  if (vector_is_empty(arr))
  {
    return NULL;
  }
  if (vector_is_oversized(arr))
  {
    vector_shrink(arr, 1);
  }
  void *item = arr->elements[arr->size - 1];
  arr->elements[--arr->size] = NULL;
  return item;
}

int
vector_is_empty(const Vector *arr)
{
  return !arr->size;
}

int
vector_is_full(const Vector *arr)
{
  return arr->size == arr->capacity;
}

static void
vector_grow(Vector *arr, size_t f)
{
  arr->capacity <<= f;
  arr->elements = mem_realloc(arr->elements, sizeof(void *) * arr->capacity);
  assert(arr->elements);
}

static void
vector_shrink(Vector *arr, size_t f)
{
  arr->capacity >>= f;
  arr->elements = mem_realloc(arr->elements, sizeof(void *) * arr->capacity);
  assert(arr->elements);
}

static int
vector_is_oversized(const Vector *arr)
{
  if (arr->size <= arr->min)
  {
    return 0;
  }

  if (4 * arr->size < arr->capacity)
  {
    return 1;
  }

  return 0;
}
