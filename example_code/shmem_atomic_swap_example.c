#include <shmem.h>
#include <stdio.h>

int main(void) {
  static long dest;
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  dest = mype;
  shmem_barrier_all();
  long new_val = mype;
  if (mype & 1) {
    long swapped_val = shmem_atomic_swap(&dest, new_val, (mype + 1) % npes);
    printf("%d: dest = %ld, swapped = %ld\n", mype, dest, swapped_val);
  }
  shmem_finalize();
  return 0;
}
