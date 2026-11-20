#pragma once

#include "../shared.h"
#include "utils/container/CircularLinkedList.h"

int
cll_has_sz(CircularList *cl, size_t sz)
{
  return cl_get_size(cl) == sz;
}

int
cll_compare_elements(CircularList *cl, int *data)
{
  size_t sz = cl_get_size(cl);
  int *elem;
  for (size_t i = 0; i < sz; i++)
  {
    elem = (int *)cl_get_item(cl, i);
    if (*elem != data[i])
    {
      return 0;
    }
  }
  return 1;
}

void
assert_cll(CircularList *cl, int *data, int size)
{
  assert(cll_has_sz(cl, size));
  assert(cll_compare_elements(cl, data));
}
