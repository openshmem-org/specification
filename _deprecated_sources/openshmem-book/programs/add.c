#include <stdio.h>

#include <shmem.h>

int counter = 0;

int
main (int argc, char *argv[])
{
  int me;

  start_pes (0);
  me = _my_pe ();

  if (me > 0)
    {
      shmem_int_add (&counter, me, 0);
    }

  shmem_barrier_all ();

  if (me == 0)
    {
      printf ("counter = %d\n", counter);
    }

  return 0;
}
