#include <stdio.h>
#include <shmem.h>

int main(void)
{
   long source[10] = { 1, 2, 3, 4, 5,
   		                 6, 7, 8, 9, 10 };
   static long dest[10];
   shmem_init();
   if (shmem_my_pe() == 0) {
      /* put 10 words into dest on PE 1 */
      shmem_put(dest, source, 10, 1);
   }
   shmem_barrier_all();  /* sync sender and receiver */
   printf("dest[0] on PE %d is %ld\n", shmem_my_pe(), dest[0]);
   shmem_finalize();
   return 0;
}

