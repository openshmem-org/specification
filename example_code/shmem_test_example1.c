#include <stdlib.h>
#include <shmem.h>

void user_wait_until(long *ivar, int cmp, long value)
{
  while (!shmem_test(ivar, cmp, value));
}
