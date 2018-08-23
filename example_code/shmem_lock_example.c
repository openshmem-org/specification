#include <stdio.h>
#include <shmem.h>

int main(void)
{
   static long lock = 0;
   static int count = 0;
   shmem_init();
   int me = shmem_my_pe();
   shmem_set_lock(&lock);
   int val = shmem_g(&count, 0); /* get count value on PE 0 */
   printf("%d: count is %d\n", me, val);
   val++; /* incrementing and updating count on PE 0 */
   shmem_p(&count, val, 0);
   shmem_clear_lock(&lock); /* ensures count update has completed before clearing the lock */
   shmem_finalize();
   return 0;
}
