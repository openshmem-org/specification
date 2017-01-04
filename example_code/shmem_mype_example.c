#include <stdio.h>
#include <shmem.h>

int main(void)
{
   shmem_init(); 
   int me = shmem_my_pe();
   printf("My PE id is: %d\n", me);
   shmem_finalize();
   return 0;
}
