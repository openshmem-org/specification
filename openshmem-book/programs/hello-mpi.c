#include <stdio.h>
#include <mpi.h>

int
main (int argc, char *argv[])
{
  int nprocs, me;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &me);
  printf ("Hello from %d of %d\n", me, nprocs);
  MPI_Finalize ();
  return 0;
}
