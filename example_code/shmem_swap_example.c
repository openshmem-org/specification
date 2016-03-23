#include <stdio.h>
#include <shmem.h>

int main(void)
{
   long *dest;
   int me, npes;
   long swapped_val, new_val;

   shmem_init();
   me = shmem_my_pe();
   npes = shmem_n_pes();
   dest = (long *) shmem_malloc(sizeof (*dest));
   *dest = me;
   shmem_barrier_all();
   new_val = me;
   if (me & 1){
      swapped_val = shmem_swap(dest, new_val, (me + 1) % npes);
      printf("%d: dest = %ld, swapped = %ld\n", me, *dest, swapped_val);
   }
   shmem_free(dest);
   return 0;
}
