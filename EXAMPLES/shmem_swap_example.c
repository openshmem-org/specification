#include <stdio.h>
#include <shmem.h>

int main(void)
{
   long *target;
   int me, npes;
   long swapped_val, new_val;

   start_pes(0);
   me = _my_pe();
   npes = _num_pes();
   target = (long *) shmalloc(sizeof (*target));
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
