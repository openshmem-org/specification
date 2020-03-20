#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int dst = 74;
  shmem_init();
  int mype = shmem_my_pe();
  if (mype == 0)
    shmem_atomic_inc(&dst, 1);
  shmem_barrier_all();
  printf("%d: dst = %d\n", mype, dst);
  shmem_finalize();
  return 0;
}
