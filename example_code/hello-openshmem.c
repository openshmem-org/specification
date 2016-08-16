#include <stdio.h>
#include <shmem.h>          /* The shmem header file */

int
main (int argc, char *argv[])
{
  int nprocs, me;

  shmem_init ();
  nprocs = shmem_n_pes ();
  me = shmem_my_pe ();
  printf ("Hello from %d of %d\n", me, nprocs);
  shmem_finalize();
  return 0;
}
