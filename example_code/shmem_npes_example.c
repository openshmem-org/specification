#include <stdio.h>
#include <shmem.h>

int main(void)
{
   shmem_init();
   int me = shmem_my_pe();
   int npes = shmem_n_pes();
   if (me == 0)
      printf("Number of PEs executing this program is: %d\n", npes);
   shmem_finalize();
   return 0;
}
