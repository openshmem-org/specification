#include <shmem.h>
#include <stdio.h>

int main(void) {
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  printf("I am #%d of %d PEs executing this program\n", mype, npes);
  shmem_finalize();
  return 0;
}
