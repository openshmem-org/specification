#include <shmem.h>

int main(void) {
  static int x = 0, y = 0;

  shmem_init();
  shmem_int_atomic_inc(&x, (shmem_my_pe() + 1) % shmem_n_pes());
  /* Undefined behavior: The following reduction operation performs accesses to
   * symmetric variable 'x' that are concurrent with previously issued atomic
   * increment operations on the same variable. */
  shmem_int_sum_reduce(SHMEM_TEAM_WORLD, &y, &x, 1);

  shmem_finalize();
  return 0;
}
