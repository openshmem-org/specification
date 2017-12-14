#include <stdio.h>
#include <shmem.h>

int main(void)
{
   static int x = 10101;
   static long pSync[SHMEM_BARRIER_SYNC_SIZE];

   shmem_init();
   int me = shmem_my_pe();
   int npes = shmem_n_pes();

   for (int i = 0; i < SHMEM_BARRIER_SYNC_SIZE; i++)
      pSync[i] = SHMEM_SYNC_VALUE;

   shmem_sync_all();

   if (me % 2 == 0) {
      /* put to next even PE in a circular fashion */
      shmem_p(&x, 4, (me + 2) % npes);
      /* synchronize all even pes */
      shmem_quiet();
      shmem_sync(0, 1, (npes / 2 + npes % 2), pSync);
   }
   printf("%d: x = %d\n", me, x);
   shmem_finalize();
   return 0;
}
