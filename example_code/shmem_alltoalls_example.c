#include <stdio.h>
#include <inttypes.h>
#include <shmem.h>

int main(void)
{
   static long pSync[SHMEM_ALLTOALLS_SYNC_SIZE];
   for (int i = 0; i < SHMEM_ALLTOALLS_SYNC_SIZE; i++)
      pSync[i] = SHMEM_SYNC_VALUE;

   shmem_init();
   int me = shmem_my_pe();
   int npes = shmem_n_pes();

   const int count = 2;
   const ptrdiff_t dst = 2;
   const ptrdiff_t sst = 3;
   int64_t* dest = (int64_t*) shmem_malloc(count * dst * npes * sizeof(int64_t));
   int64_t* source = (int64_t*) shmem_malloc(count * sst * npes * sizeof(int64_t));

   /* assign source values */
   for (int pe = 0; pe < npes; pe++) {
      for (int i = 0; i < count; i++) {
         source[sst * ((pe * count) + i)] = me + pe;
         dest[dst * ((pe * count) + i)] = 9999;
      }
   }
   /* wait for all PEs to update source/dest */
   shmem_barrier_all();

   /* alltoalls on all PES */
   shmem_alltoalls64(dest, source, dst, sst, count, 0, 0, npes, pSync);

   /* verify results */
   for (int pe = 0; pe < npes; pe++) {
      for (int i = 0; i < count; i++) {
         int j = dst * ((pe * count) + i);
         if (dest[j] != pe + me) {
            printf("[%d] ERROR: dest[%d]=%" PRId64 ", should be %d\n",
               me, j, dest[j], pe + me);
          }
       }
   }

   shmem_free(dest);
   shmem_free(source);
   shmem_finalize();
   return 0;
}
