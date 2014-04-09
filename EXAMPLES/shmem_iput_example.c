#include <stdio.h>
#include <shmem.h>

int main(void)
{
   short source[10] = { 1, 2, 3, 4, 5,
                        6, 7, 8, 9, 10 };
   static short target[10];
   start_pes(0);
   if (_my_pe() == 0) {
      /* put 10 words into target on PE 1 */
      shmem_short_iput(target, source, 1, 2, 5, 1);
   }
   shmem_barrier_all();   /* sync sender and receiver */
   if (_my_pe() == 1) {
      printf("target on PE %d is %d %d %d %d %d0, _my_pe(),
      (int)target[0], (int)target[1], (int)target[2],
      (int)target[3], (int)target[4] );
   }
   shmem_barrier_all();   /* sync before exiting */
   return 1;
}
