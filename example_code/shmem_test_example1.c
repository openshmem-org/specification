#include <stdlib.h>
#include <shmem.h>

void user_wait(long *ivar, long value)
{
  while (shmem_test(ivar, value));
}
