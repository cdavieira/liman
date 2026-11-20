#include "./util.h"

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
