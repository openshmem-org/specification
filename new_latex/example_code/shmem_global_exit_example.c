#include <stdio.h>
#include <shmem.h> 

int main(void) {
  int me, npes; 

   shmem_init();

   me = shmem_my_pe();
   npes = shmem_n_pes();

   if (me == 0){

     fp = fopen(input.txt,"r"); 
     if( fp == NULL ){          /*Input file required by program is not available*/
       status = EXIT_FAILURE;
       shmem_global_exit(status);
     }
   }

   fclose(fp);

  return 0;
}

