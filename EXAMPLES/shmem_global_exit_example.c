#include <stdio.h>
#include <shmem.h> 

int main(void) {
  int me, npes; 

   shmem_init();

   me = shmem_my_pe();
   npes = shmem_num_pes();
   if (me == 0){
     char ch;
     FILE *fp;
     int status;

     fp = fopen(input.txt,"r"); 
     if( fp == NULL ){                       /*Input file required by application is not available*/
       status = EXIT_FAILURE;
       shmem_global_exit(status);
     }
   }
  ...
  ...
  return 0;
}

