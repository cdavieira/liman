#include "utils/container/Vector.h"
#include "./util.h"
#include <assert.h>

static void test_append_pop(void);
static void test_capacity_doubling(void);
static void test_capacity_halving(void);

int
main(int argc, char *argv[])
{
  test_append_pop();
  test_capacity_doubling();
  test_capacity_halving();

  return 0;
}

static void
test_append_pop(void)
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

  v = vector_destroy(v, dummy_free);
}

static void
test_capacity_doubling(void)
{
  Vector *v = vector_new(8);
  int value = 0;

  for (int i = 0; i < 8; i++)
  {
    vector_append(v, &value);
  }

  assert(vector_is_full(v));
  assert(vector_get_capacity(v) == 8);

  vector_append(v, &value);

  assert(!vector_is_full(v));
  assert(vector_get_capacity(v) == 16);

  for (int i = 9; i > 0; i--)
  {
    vector_pop(v);
  }

  assert(vector_is_empty(v));
  assert(vector_get_capacity(v) == 16);

  v = vector_destroy(v, dummy_free);
}

static void
test_capacity_halving(void)
{
  Vector *v = vector_new(4);
  int value = 1;

  for (int i = 0; i < 32; i++)
  {
    vector_append(v, &value);
  }

  assert(vector_get_capacity(v) == 32);

  for (int i = 25; i > 0; i--)
  {
    vector_pop(v);
  }

  assert(vector_get_capacity(v) == 16);
}
