#include <stdio.h>
#include <shmem.h>

int main(void)
{
   static long dest;
   shmem_init();
   int me = shmem_my_pe();
   int npes = shmem_n_pes();
   dest = me;
   shmem_barrier_all();
   long new_val = me;
   if (me & 1) {
      long swapped_val = shmem_swap(&dest, new_val, (me + 1) % npes);
      printf("%d: dest = %ld, swapped = %ld\n", me, dest, swapped_val);
   }
   shmem_finalize();
   return 0;
}
