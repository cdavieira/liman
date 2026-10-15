#include "utils/container/Vector.h"
#include <assert.h>
#include <stddef.h>

static int vector_has_sz(Vector *v, size_t sz);

static int vector_compare_elements(Vector *v, int *data);

static void assert_vector(Vector *v, int *data, int size);

void *
dummy(void *elem)
{
  return NULL;
}

int
main(int argc, char *argv[])
{
  Vector *v = vector_new(8);
  int arr[10];

  for (int i = 0; i < 10; i++)
  {
    arr[i] = i + 1;
    vector_append(v, arr + i);
    assert_vector(v, arr, i + 1);
  }

  for (int i = 9; i > 0; i--)
  {
    vector_pop(v);
    assert_vector(v, arr, i);
  }

  v = vector_destroy(v, dummy);

  return 0;
}

static int
vector_has_sz(Vector *v, size_t sz)
{
  return vector_get_size(v) == sz;
}

static int
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

static void
assert_vector(Vector *v, int *data, int size)
{
  assert(vector_has_sz(v, size));
  assert(vector_compare_elements(v, data));
}
