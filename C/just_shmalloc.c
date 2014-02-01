/*
 * just do a shmalloc and a free, no output to be expected
 */

#include <shmem.h>

int
main()
{
  long *x;

  start_pes(0);

  x = (long *) shmalloc(sizeof(*x));

  shfree(x);
  shmem_barrier_all();

  return 0;
}

