#include <stdio.h>
#include <shmem.h>

int main(void)
{
   short source[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
   static short dest[10];
   shmem_init();
   int me = shmem_my_pe();
   if (me == 0) /* put 5 words into dest on PE 1 */
      shmem_iput(dest, source, 1, 2, 5, 1);
   shmem_barrier_all(); /* sync sender and receiver */
   if (me == 1) {
      printf("dest on PE %d is %hd %hd %hd %hd %hd\n", me,
         dest[0], dest[1], dest[2], dest[3], dest[4]);
   }
   shmem_finalize();
   return 0;
}
