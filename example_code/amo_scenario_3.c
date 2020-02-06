#include <shmem.h>

int main(void) {
  static long psync[SHMEM_REDUCE_SYNC_SIZE];
  static int pwrk[SHMEM_REDUCE_MIN_WRKDATA_SIZE];
  static int x = 0, y = 0;

  for (int i = 0; i < SHMEM_REDUCE_SYNC_SIZE; i++)
    psync[i] = SHMEM_SYNC_VALUE;

  shmem_init();
  shmem_int_atomic_inc(&x, (shmem_my_pe() + 1) % shmem_n_pes());
  /* Undefined behavior: The following reduction operation performs accesses to
   * symmetric variable 'x' that are concurrent with previously issued atomic
   * increment operations on the same variable. */
  shmem_int_sum_to_all(&y, &x, 1, 0, 0, shmem_n_pes(), pwrk, psync);

  shmem_finalize();
  return 0;
}
