#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  shmem_init();
  int mype = shmem_my_pe();
  if (mype == 0) {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) { /* Input file required by program is not available */
      shmem_global_exit(EXIT_FAILURE);
    }
    /* do something with the file */
    fclose(fp);
  }
  shmem_finalize();
  return 0;
}
