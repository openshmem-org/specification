#include <stdio.h>
#include <shmem.h>

int main(void)
{
   int old = -1;
   static int dst = 22;
   static long lock = 0;
   shmem_init();
   int me = shmem_my_pe();
   if (me == 1) {
      shmem_set_lock_pe(&lock, 0);
      old = shmem_g(&dst, 0);
      shmem_p(&dst, old + 44, 0);
      shmem_clear_lock_pe(&lock, 0);
   }
   shmem_barrier_all();
   printf("%d: old = %d, dst = %d\n", me, old, dst);
   shmem_finalize();
   return 0;
}
