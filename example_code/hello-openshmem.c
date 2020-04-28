#include <shmem.h> /* The OpenSHMEM header file */
#include <stdio.h>

int main(void) {
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  printf("Hello from %d of %d\n", mype, npes);
  shmem_finalize();
  return 0;
}
