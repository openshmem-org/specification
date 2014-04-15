#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

long pSync[_SHMEM_COLLECT_SYNC_SIZE];
int source[2];

int main(void)
{
   int i, me, npes;
   int *target;

   start_pes(0);
   me = _my_pe();
   npes = _num_pes();

   source[0] = me * 2;
   source[1] = me * 2 + 1;
   target = (int *)shmalloc(sizeof(int) * npes * 2);
   for (i=0; i < _SHMEM_COLLECT_SYNC_SIZE; i++) {
      pSync[i] = _SHMEM_SYNC_VALUE;
   }
   shmem_barrier_all(); /* Wait for all PEs to initialize pSync */

   shmem_collect32(target, source, 2, 0, 0, npes, pSync);
   printf("%d: %d", me, target[0]);
   for (i = 1; i < npes * 2; i++)
      printf(", %d", target[i]);
   printf("\n");
   return 0;
}
