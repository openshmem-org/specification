#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#define NUM_ELEMS 4
long pSync[_SHMEM_BCAST_SYNC_SIZE];
long source[NUM_ELEMS], dest[NUM_ELEMS];

int main(void)
{
   int i, me, npes;

   shmem_init();
   me = shmem_my_pe();
   npes = shmem_n_pes();

   if (me == 0)
      for (i = 0; i < NUM_ELEMS; i++)
         source[i] = i;
   for (i=0; i < _SHMEM_BCAST_SYNC_SIZE; i++) {
      pSync[i] = _SHMEM_SYNC_VALUE;
   }
   shmem_barrier_all(); /* Wait for all PEs to initialize pSync */

   shmem_broadcast64(dest, source, NUM_ELEMS, 4, 5, 0, 3, pSync);
   printf("%d: %ld", me, dest[0]);
   for (i = 1; i < NUM_ELEMS; i++)
      printf(", %ld", dest[i]);
   printf("\n");
   return 0;
}
