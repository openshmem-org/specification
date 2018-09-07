#include <shmem.h>
#include <stdlib.h>

#define N 100

int main(void)
{
  int total_sum = 0;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int my_data = malloc(N * sizeof(int));
  int *all_data = shmem_malloc(N * npes * sizeof(int));

  int *flags = shmem_calloc(npes, sizeof(int));
  int *status = calloc(npes, sizeof(int));

  for (int i = 0; i < N; i++)
      my_data[i] = mype*N + i;

  for (int i = 0; i < npes; i++)
      shmem_put_nbi(&all_data[mype*N], my_data, N, i);

  shmem_fence();

  for (int i = 0; i < npes; i++)
      shmem_p(&flags[mype], 1, i);
  
  int ncompleted = 0;

  while (ncompleted < npes) {
      int completed_idx = shmem_test_any(flags, npes, status, SHMEM_CMP_NE, 0); 
      if (completed_idx > 0) {
          for (int j = 0; j < N; j++)
              total_sum += all_data[completed_idx * N + j];
          ncompleted++;
      } else {
          /* Overlap some computation here */
      }
  }

  int M = N * npes - 1;
  if (total_sum != M * (M + 1) / 2) shmem_global_exit(1);

  free(my_data);
  free(status);
  shmem_free(all_data);
  shmem_free(flags);
  shmem_finalize();
  return 0;
}
