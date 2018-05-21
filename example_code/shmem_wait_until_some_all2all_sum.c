#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>
#include <string.h>
#include <assert.h>

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
  _Bool *completed = calloc(npes, sizeof(_Bool));
  _Bool *processed = calloc(npes, sizeof(_Bool));

  for (int i = 0; i < N; i++)
    my_data[i] = mype*N + i;

  for (int i = 0; i < npes; i++)
    shmem_put_nbi(&all_data[i*N], my_data, N, i);

  shmem_fence();

  for (int i = 0; i < npes; i++)
    shmem_p(&flags[i], 1, i);
  
  while (shmem_wait_until_some(flags, npes, completed, SHMEM_CMP_NE, 0)) {

    for (int i = 0; i < npes; i++) {
      if (completed[i] == 1 && !processed[i] == 1) {
        for (int j = 0; j < N; j++)
          total_sum += all_data[i*N+ j];
        processed[i] = 1;
      }
    }
  }

  int M = N * npes - 1;
  assert(total_sum == M * (M + 1) / 2);

  free(my_data);
  free(completed);
  shmem_free(all_data);
  shmem_free(flags);
  shmem_free(processed);
  shmem_finalize();
  return 0;
}
