#include <stdio.h>
#include <shmem.h>

#define SIZE 16

int main(void)
{
   short source[SIZE];
   static short dest[SIZE];
   static long lock = 0;
   shmem_init();
   int me = shmem_my_pe();
   int npes = shmem_n_pes();
   if (me == 0) {
      /* initialize array */
      for (int i = 0; i < SIZE; i++)
         source[i] = i;
      /* local, not symmetric */
      /* static makes it symmetric */
      /* put "size" words into dest on each PE */
      for (int i = 1; i < npes; i++)
         shmem_put(dest, source, SIZE, i);
   }
   shmem_barrier_all(); /* sync sender and receiver */
   if (me != 0) {
      shmem_set_lock(&lock);
      printf("dest on PE %d is \t", me);
      for (int i = 0; i < SIZE; i++)
         printf("%hd \t", dest[i]);
      printf("\n");
      shmem_clear_lock(&lock);
   }
   shmem_finalize();
   return 0;
}
