#include <shmem.h>

int user_wait_any(long *ivar, int count, shmem_cmp cmp, long value)
{
  int idx = 0;
  while (!shmem_test(&ivar[idx], cmp, value))
    idx = (idx + 1) % count;
  return idx;
}
