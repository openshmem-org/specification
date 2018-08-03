#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int dst = 74;
  shmem_init();
  int me = shmem_my_pe();
  if (me == 0)
    shmem_atomic_inc(&dst, 1);
  shmem_barrier_all();
  printf("%d: dst = %d\n", me, dst);
  shmem_finalize();
  return 0;
}
