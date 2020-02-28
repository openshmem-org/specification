#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#define NELEMS 32

int main(void) {
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int *values = shmem_malloc(NELEMS * sizeof(int));

  static int my_maximal_values = 0;
  static int all_maximal_values;

  srand((unsigned)mype);

  for (int i = 0; i < NELEMS ; i++) {
    values[i] = rand() % npes;

    /* Count the instances of maximal values (i.e., equal to (npes-1)) */
    my_maximal_values += (values[i] == (npes - 1)) ? 1 : 0;
  }

  /* Wait for all PEs to initialize reductions arrays */
  shmem_sync(SHMEM_TEAM_WORLD);

  shmem_sum_reduce(SHMEM_TEAM_WORLD, &all_maximal_values, &my_maximal_values, 1);

  if (mype == 0)
    printf("found a total of %d maximal random numbers\n", all_maximal_values);

  shmem_finalize();
  return 0;
}
