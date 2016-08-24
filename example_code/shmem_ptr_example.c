#include <stdio.h>
#include <shmem.h>

int main(void) 
{
   static int bigd[100];
      int *ptr;
      int i;

   shmem_init();

   if (shmem_my_pe() == 0) {
      /* initialize PE 1's bigd array */
      ptr = shmem_ptr(bigd, 1);
      if (ptr == NULL)
         printf("can't use pointer to directly access PE 1's array\n");
      else
         for (i=0; i<100; i++)
            *ptr++ = i+1;
   }

   shmem_barrier_all();

   if (shmem_my_pe() == 1) {
      printf("bigd on PE 1 is:\n");
      for (i=0; i<100; i++)
         printf(" %d\n",bigd[i]);
      printf("\n");
   }
   shmem_finalize();
   return 0;
}
