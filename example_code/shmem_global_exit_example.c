#include <stdio.h>
#include <stdlib.h>
#include <shmem.h> 

int main(void)
{
   shmem_init();
   int me = shmem_my_pe();
   if (me == 0) {
      FILE *fp = fopen("input.txt", "r"); 
      if (fp == NULL) {  /* Input file required by program is not available */
         shmem_global_exit(EXIT_FAILURE);
      }
      /* do something with the file */
      fclose(fp);
  }
  shmem_finalize();
  return 0;
}
