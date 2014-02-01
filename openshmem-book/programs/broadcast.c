#include <stdio.h>
#include <stdlib.h>

#include <shmem.h>

long pSync[_SHMEM_BCAST_SYNC_SIZE];

int
main (void)
{
  long *target, *source;
  int i, me, npes;

  start_pes (0);
  me = _my_pe ();
  npes = _num_pes ();

  source = (long *) shmalloc (npes * sizeof (*source));
  for (i = 0; i < npes; i += 1)
    {
      source[i] = i + 1;
    }
  target = (long *) shmalloc (npes * sizeof (*target));
  for (i = 0; i < npes; i += 1)
    {
      target[i] = -999;
    }
  for (i = 0; i < _SHMEM_BCAST_SYNC_SIZE; i += 1)
    {
      pSync[i] = _SHMEM_SYNC_VALUE;
    }
  shmem_barrier_all ();

  shmem_broadcast64 (target, source, npes, 0, 0, 0, npes, pSync);

  for (i = 0; i < npes; i++)
    {
      printf ("%-8d %ld\n", me, target[i]);
    }

  shmem_barrier_all ();
  shfree (target);
  shfree (source);

  return 0;
}
