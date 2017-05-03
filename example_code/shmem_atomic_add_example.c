#include <stdio.h>
#include <shmem.h>

int main(void)
{
   static int dst = 22;
   shmem_init();
   int me = shmem_my_pe();
   if (me == 1)
      shmem_atomic_add(&dst, 44, 0);
   shmem_barrier_all();
   printf("%d: dst = %d\n", me, dst);
   shmem_finalize();
   return 0;
}
