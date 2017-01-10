#include <stdio.h>
#include <shmem.h>

long pSync[SHMEM_BARRIER_SYNC_SIZE];
int x = 10101;

int main(void)
{
   int i, me, npes;

   for (i = 0; i < SHMEM_BARRIER_SYNC_SIZE; i += 1){
      pSync[i] = SHMEM_SYNC_VALUE;
   }

   shmem_init();
   me = shmem_my_pe();
   npes = shmem_n_pes();

   if(me % 2 == 0){
      x = 1000 + me;
      /*put to next even PE in a circular fashion*/
      shmem_int_p(&x, 4, (me+2)%npes);
      /*synchronize all even pes*/
      shmem_barrier(0, 1, (npes/2 + npes%2), pSync);
   }
   printf("%d: x = %d\n", me, x);
   shmem_finalize();
   return 0;
}
