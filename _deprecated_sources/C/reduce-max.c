/*
 * reduce [0,1,2] + _my_pe() across 4 PEs with MAX()
 *
 *
 */

#include <stdio.h>
#include <string.h>

#include <shmem.h>

long pSync[_SHMEM_BCAST_SYNC_SIZE];

#define N 3

long src[N];
long dst[N];
long pWrk[_SHMEM_REDUCE_SYNC_SIZE];

int
main(void)
{
   int i;

   for (i = 0; i < SHMEM_BCAST_SYNC_SIZE; i += 1) {
      pSync[i] = _SHMEM_SYNC_VALUE;
   }

   start_pes(0);

   for (i = 0; i < N; i += 1) {
      src[i] = _my_pe() + i;
   }
   shmem_barrier_all();

   shmem_long_max_to_all(dst, src, N, 0, 0, _num_pes(), pWrk, pSync);

   printf("%d/%d   dst =", _my_pe(), _num_pes() );
   for (i = 0; i < N; i+= 1) {
      printf(" %ld", dst[i]);
   }
   printf("\n");

   return 0;
}
