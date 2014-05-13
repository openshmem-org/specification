#include <stdio.h>
#include <shmem.h>

int main(void)
{
   long *target;
   int me, npes;
   long swapped_val, new_val;

   shmem_init();
   me = shmem_my_pe();
   npes = shmem_n_pes();
   target = (long *) shmem_malloc(sizeof (*target));
   *target = me;
   shmem_barrier_all();
   new_val = me;
   if (me & 1){
      swapped_val = shmem_long_swap(target, new_val, (me + 1) % npes);
      printf("%d: target = %ld, swapped = %ld\n", me, *target, swapped_val);
   }
   shfree(target);
   return 0;
}
