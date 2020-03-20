#include <shmem.h>
#include <stdio.h>

int main(void) {
  int old = -1;
  static int dst = 22;
  shmem_init();
  int mype = shmem_my_pe();
  if (mype == 0)
    old = shmem_atomic_fetch_inc(&dst, 1);
  shmem_barrier_all();
  printf("%d: old = %d, dst = %d\n", mype, old, dst);
  shmem_finalize();
  return 0;
}
