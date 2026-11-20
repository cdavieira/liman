#pragma once

#include "../shared.h"
#include "utils/container/Vector.h"

int
vector_has_sz(Vector *v, size_t sz)
{
  return vector_get_size(v) == sz;
}

int
vector_compare_elements(Vector *v, int *data)
{
  size_t sz = vector_get_size(v);
  int *elem;
  for (size_t i = 0; i < sz; i++)
  {
    elem = (int *)vector_get_item(v, i);
    if (*elem != data[i])
    {
      return 0;
    }
  }
  return 1;
}

void
assert_vector(Vector *v, int *data, int size)
{
  assert(vector_has_sz(v, size));
  assert(vector_compare_elements(v, data));
}
