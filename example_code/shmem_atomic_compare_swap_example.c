#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int race_winner = -1;
  shmem_init();
  int mype = shmem_my_pe();
  int oldval = shmem_atomic_compare_swap(&race_winner, -1, mype, 0);
  if (oldval == -1)
    printf("PE %d was first\n", mype);
  shmem_finalize();
  return 0;
}
