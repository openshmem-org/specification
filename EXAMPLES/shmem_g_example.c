#include <stdio.h>
#include <shmem.h>

long x = 10101;

int main(void)
{
   int me, npes;
   long y = -1;

   shmem_init();
   me = shmem_my_pe();
   npes = shmem_n_pes();

   if (me == 0)
      y = shmem_long_g(&x, npes-1);

   printf("%d: y = %ld\n", me, y);

   return 0;
}
