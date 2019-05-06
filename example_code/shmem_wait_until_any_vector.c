#include <shmem.h>
#include <stdlib.h>

#define N 100

int main(void)
{
  int total_sum = 0;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int *ivars = shmem_calloc(npes, sizeof(int));
  int *status = calloc(npes, sizeof(int));
  int *cmp_values = malloc(npes * sizeof(int));

  /* All odd PEs put 2 and all even PEs put 1 */
  for (int i = 0; i < npes; i++) {
      shmem_p(&ivars[mype], mype % 2 + 1, i);

      /* Set cmp_values to the expected values coming from each PE */
      cmp_values[i] = i % 2 + 1;
  }

  for (int i = 0; i < npes; i++) {
      size_t completed_idx = shmem_wait_until_any_vector(ivars, npes, status,
                                                         SHMEM_CMP_EQ, cmp_values);
      status[completed_idx] = 1;
      total_sum += ivars[completed_idx];
  }

  /* check the result */
  int correct_result = npes + npes / 2 + npes % 2;

  if (total_sum != correct_result) {
      shmem_global_exit(1);
  }

  shmem_finalize();
  return 0;
}
