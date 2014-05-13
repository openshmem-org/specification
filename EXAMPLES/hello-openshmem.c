#include <stdio.h>
#include <shmem.h>          /* The shmem header file */

int
main (int argc, char *argv[])
{
  int nprocs, me;

  shmem_init (0);
  nprocs = shmem_n_pes ();
  me = shmem_my_pe ();
  printf ("Hello from %d of %d\n", me, nprocs);
  return 0;
}
