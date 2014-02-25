#include <stdio.h>
#include <shmem.h>
long x = 10101;

int main(void)
{
   int me, npes;
   long y = -1;

   start_pes(0);
   me = _my_pe();
   npes = _num_pes();

   if (me == 0)
      y = shmem_long_g(&x, 1);

   printf("%d: y = %ld\n", me, y);

   return 0;
}
