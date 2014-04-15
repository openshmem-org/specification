#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#define NUM_ELEMS 4
long pSync[_SHMEM_BCAST_SYNC_SIZE];
long source[NUM_ELEMS], target[NUM_ELEMS];

int main(void)
{
   int i, me, npes;

   start_pes(0);
   me = _my_pe();
   npes = _num_pes();

   if (me == 0)
      for (i = 0; i < NUM_ELEMS; i++)
         source[i] = i;
   for (i=0; i < _SHMEM_BCAST_SYNC_SIZE; i++) {
      pSync[i] = _SHMEM_SYNC_VALUE;
   }
   shmem_barrier_all(); /* Wait for all PEs to initialize pSync */

   shmem_broadcast64(target, source, NUM_ELEMS, 0, 0, 0, npes, pSync);
   printf("%d: %ld", me, target[0]);
   for (i = 1; i < NUM_ELEMS; i++)
      printf(", %ld", target[i]);
   printf("\n");
   return 0;
}
