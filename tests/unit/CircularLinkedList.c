#include "utils/container/CircularLinkedList.h"
#include <assert.h>
#include <stddef.h>

static int cll_has_sz(CircularList *cl, size_t sz);

static int cll_compare_elements(CircularList *cl, int *data);

static void assert_cll(CircularList *cl, int *data, int size);

static void *
dummy_free(void *e)
{
  return NULL;
}

static void test_add_rm(void);

int
main(int argc, char *argv[])
{
  test_add_rm();

  return 0;
}

static void
test_add_rm(void)
{
  CircularList *cl = cl_new();
  int arr[10];

  assert_cll(cl, arr, 0);

  for (int i = 0; i < 10; i++)
  {
    arr[i] = i + 1;
    cl_add(cl, arr + i, i);
    assert_cll(cl, arr, i + 1);
  }

  for (int i = 9; i > 0; i--)
  {
    cl_remove(cl, i);
    assert_cll(cl, arr, i);
  }

  cl = cl_destroy(cl, dummy_free);
}

static int
cll_has_sz(CircularList *cl, size_t sz)
{
  return cl_get_size(cl) == sz;
}

static int
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

static void
assert_cll(CircularList *cl, int *data, int size)
{
  assert(cll_has_sz(cl, size));
  assert(cll_compare_elements(cl, data));
}
