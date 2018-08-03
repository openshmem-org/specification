#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int dest[4];
  shmem_init();
  int me = shmem_my_pe();
  if (me == 0) { /* initialize PE 1's dest array */
    int *ptr = shmem_ptr(dest, 1);
    if (ptr == NULL)
      printf("can't use pointer to directly access PE 1's dest array\n");
    else
      for (int i = 0; i < 4; i++)
        *ptr++ = i + 1;
  }
  shmem_barrier_all();
  if (me == 1)
    printf("PE 1 dest: %d, %d, %d, %d\n", dest[0], dest[1], dest[2], dest[3]);
  shmem_finalize();
  return 0;
}
