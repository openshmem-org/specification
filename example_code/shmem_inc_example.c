#include <stdio.h>
#include <shmem.h>

int dst;

int main(void)
{
   int me;

   shmem_init();
   me = shmem_my_pe();

   dst = 74;
   shmem_barrier_all();

   if (me == 0)
      shmem_int_inc(&dst, 1);
   shmem_barrier_all();

   printf("%d: dst = %d\n", me, dst);
   return 0;
}
