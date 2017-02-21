#include <stdio.h>
#include <shmem.h>

int main(void)
{
   int me, old;
   static int dst;

   shmem_init();
   me = shmem_my_pe();

   old = -1;
   dst = 22;
   shmem_barrier_all();

   if (me == 1){
      old = shmem_atomic_fetch_add(&dst, 44, 0);
   }
   shmem_barrier_all();
   printf("%d: old = %d, dst = %d\n", me, old, dst);
   shmem_finalize();
   return 0;
}
