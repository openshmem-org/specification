#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int dst = 22;
  shmem_init();
  int mype = shmem_my_pe();
  if (mype == 1)
    shmem_atomic_add(&dst, 44, 0);
  shmem_barrier_all();
  printf("%d: dst = %d\n", mype, dst);
  shmem_finalize();
  return 0;
}
