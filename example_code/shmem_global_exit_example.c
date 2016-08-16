#include <stdio.h>
#include <stdlib.h>
#include <shmem.h> 

int
main(void)
{
  int me, npes; 

  shmem_init();

  me = shmem_my_pe();
  npes = shmem_n_pes();

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
