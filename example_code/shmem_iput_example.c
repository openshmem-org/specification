#include <stdio.h>
#include <shmem.h>

int main(void)
{
   short source[10] = { 1, 2, 3, 4, 5,
                        6, 7, 8, 9, 10 };
   static short dest[10];
   shmem_init();
   if (shmem_my_pe() == 0) {
      /* put 5 words into dest on PE 1 */
      shmem_iput(dest, source, 1, 2, 5, 1);
   }
   shmem_barrier_all();   /* sync sender and receiver */
   if (shmem_my_pe() == 1) {
      printf("dest on PE %d is %d %d %d %d %d\n", shmem_my_pe(),
      (int)dest[0], (int)dest[1], (int)dest[2],
      (int)dest[3], (int)dest[4] );
   }
   shmem_barrier_all();   /* sync before exiting */
   return 1;
}
