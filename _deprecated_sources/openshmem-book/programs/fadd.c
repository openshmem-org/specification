#include <stdio.h>

#include <shmem.h>

int counter = 9;		/* just some starting value */

int
main (int argc, char *argv[])
{
  int me;
  int pe_counter;

  start_pes (0);
  me = _my_pe ();

  if (me == 1)
    {
      pe_counter = shmem_int_fadd (&counter, me, 0);
      printf ("PE %d: PE 0 returned %d\n", me, pe_counter);
    }

  shmem_barrier_all ();

  if (me == 0)
    {
      printf ("PE %d: counter = %d\n", me, counter);
    }

  return 0;
}
