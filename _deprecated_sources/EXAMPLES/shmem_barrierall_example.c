#include <stdio.h>
#include <shmem.h>

int x=1010;

int main(void)
{
   int me, npes;

   shmem_init();
   me = shmem_my_pe();
   npes = shmem_n_pes();

   /*put to next  PE in a circular fashion*/
   shmem_int_p(&x, 4, (me+1)%npes);
   /*synchronize all PEs*/
   shmem_barrier_all();

   printf("%d: x = %d\n", me, x);
   return 0;
}
