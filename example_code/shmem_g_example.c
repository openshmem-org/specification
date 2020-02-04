#include <shmem.h>
#include <stdio.h>

int main(void) {
  long y = -1;
  static long x = 10101;
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  if (mype == 0)
    y = shmem_g(&x, npes - 1);
  printf("%d: y = %ld\n", mype, y);
  shmem_finalize();
  return 0;
}
