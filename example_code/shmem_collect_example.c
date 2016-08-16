#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

long pSync[SHMEM_COLLECT_SYNC_SIZE];
int source[2];

int main(void)
{
   int i, me, npes;
   int *dest;

   shmem_init();
   me = shmem_my_pe();
   npes = shmem_n_pes();

   source[0] = me * 2;
   source[1] = me * 2 + 1;
   dest = (int *)shmem_malloc(sizeof(int) * npes * 2);
   for (i=0; i < SHMEM_COLLECT_SYNC_SIZE; i++) {
      pSync[i] = SHMEM_SYNC_VALUE;
   }
   shmem_barrier_all(); /* Wait for all PEs to initialize pSync */

   shmem_collect32(dest, source, 2, 0, 0, npes, pSync);
   printf("%d: %d", me, dest[0]);
   for (i = 1; i < npes * 2; i++)
      printf(", %d", dest[i]);
   printf("\n");
   shmem_finalize();
   return 0;
}
