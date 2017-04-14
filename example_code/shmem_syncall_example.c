#include <stdio.h>
#include <shmem.h>

int main(void)
{
   static int x = 1010;

   shmem_init();
   int me = shmem_my_pe();
   int npes = shmem_n_pes();

   /* put to next  PE in a circular fashion */
   shmem_p(&x, 4, (me + 1) % npes);

   /* synchronize all PEs */
   shmem_quiet();
   shmem_sync_all();
   printf("%d: x = %d\n", me, x);
   shmem_finalize();
   return 0;
}
