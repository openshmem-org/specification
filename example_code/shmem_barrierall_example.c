#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int x = 1010;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  /* put to next  PE in a circular fashion */
  shmem_p(&x, 4, (mype + 1) % npes);

  /* synchronize all PEs */
  shmem_barrier_all();
  printf("%d: x = %d\n", mype, x);
  shmem_finalize();
  return 0;
}
