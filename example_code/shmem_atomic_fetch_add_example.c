#include <stdio.h>
#include <shmem.h>

int main(void)
{
   int old = -1;
   static int dst = 22;
   shmem_init();
   int me = shmem_my_pe();
   if (me == 1)
      old = shmem_atomic_fetch_add(&dst, 44, 0);
   shmem_barrier_all();
   printf("%d: old = %d, dst = %d\n", me, old, dst);
   shmem_finalize();
   return 0;
}
