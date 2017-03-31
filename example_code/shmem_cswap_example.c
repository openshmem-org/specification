#include <stdio.h>
#include <shmem.h>

int main(void)
{
   static int race_winner = -1;
   shmem_init();
   int me = shmem_my_pe();
   int oldval = shmem_cswap(&race_winner, -1, me, 0);
   if (oldval == -1) printf("PE %d was first\n", me);
   shmem_finalize();
   return 0;
}
