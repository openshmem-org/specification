#include <shmem.h>
#include <stdio.h>

int main(void) {
  long y = -1;
  static long x = 10101;
  shmem_init();
  int me = shmem_my_pe();
  int npes = shmem_n_pes();
  if (me == 0)
    y = shmem_g(&x, npes - 1);
  printf("%d: y = %ld\n", me, y);
  shmem_finalize();
  return 0;
}
