#include <stdio.h>
#include <shmem.h>

int main(void) 
{
   int me;
   static int dst;

   shmem_init();
   me = shmem_my_pe();

   dst = 22;
   shmem_barrier_all();

   if (me == 1){
      shmem_add(&dst, 44, 0);
   }
   shmem_barrier_all();
   printf("%d: dst = %d\n", me, dst);
   shmem_finalize();
   return 0;
}
