#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int targ = 0;

  shmem_init();
  int mype = shmem_my_pe();
  int receiver = 1 % shmem_n_pes();

  if (mype == 0) {
    int src = 33;
    shmem_put(&targ, &src, 1, receiver);
  }

  shmem_barrier_all(); /* Synchronizes sender and receiver */

  if (mype == receiver)
    printf("PE %d targ=%d (expect 33)\n", mype, targ);

  shmem_finalize();
  return 0;
}
