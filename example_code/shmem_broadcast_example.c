#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

int main(void)
{
   static long source[4], dest[4];

   shmem_init();
   int me = shmem_my_pe();
   int npes = shmem_n_pes();

   if (me == 0)
      for (int i = 0; i < 4; i++)
         source[i] = i;

   shmem_broadcast(SHMEM_TEAM_WORLD, dest, source, 4, 0);

   printf("%d: %ld, %ld, %ld, %ld\n", me, dest[0], dest[1], dest[2], dest[3]);
   shmem_finalize();
   return 0;
}
