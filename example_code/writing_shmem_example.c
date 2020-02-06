#include <shmem.h>
#include <stdio.h>

#define N 16

int main(void) {
  short source[N];
  static short dest[N];
  static long lock = 0;
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  if (mype == 0) {
    /* initialize array */
    for (int i = 0; i < N; i++)
      source[i] = i;
    /* local, not symmetric */
    /* static makes it symmetric */
    /* put "size" words into dest on each PE */
    for (int i = 1; i < npes; i++)
      shmem_put(dest, source, N, i);
  }
  shmem_barrier_all(); /* sync sender and receiver */
  if (mype != 0) {
    shmem_set_lock(&lock);
    printf("dest on PE %d is \t", mype);
    for (int i = 0; i < N; i++)
      printf("%hd \t", dest[i]);
    printf("\n");
    shmem_clear_lock(&lock);
  }
  shmem_finalize();
  return 0;
}
