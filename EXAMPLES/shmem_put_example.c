#include <stdio.h>
#include <shmem.h>
int main(void)
{
   long source[10] = { 1, 2, 3, 4, 5,
   		                 6, 7, 8, 9, 10 };
   static long target[10];
   start_pes(0);
   if (_my_pe() == 0) {
      /* put 10 words into target on PE 1 */
      shmem_long_put(target, source, 10, 1);
   }
   shmem_barrier_all();  /* sync sender and receiver */
   printf("target[0] on PE %d is %d\n", _my_pe(), target[0]);
   return 1;
}

