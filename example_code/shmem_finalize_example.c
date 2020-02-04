#include <shmem.h>
#include <stdio.h>

int main(void) {
  static long x = 10101;
  long y = -1;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (mype == 0)
    y = shmem_g(&x, npes - 1);

  printf("%d: y = %ld\n", mype, y);

  shmem_finalize();
  return 0;
}
