#include <stdio.h>
#include <shmem.h>

int main(void)
{
  int npes;

  shmem_init();
  
  npes = shmem_n_pes();

  if (shmem_my_pe() == 0) {
    printf("Number of PEs executing this application is: %d\n", npes);
  }

  return 0;
}
