#include <shmem.h>
#include <stdio.h>

#define SIZE 10

int main(void) {
  int count = 0;
  shmem_init();
  static int64_t sig_addr = 0;
  int *dest = shmem_malloc (SIZE * sizeof(int));
  int *source = shmem_malloc (SIZE * sizeof(int));
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  for (int i = 0; i < SIZE; i++) {
    source[i] = count;
    dest[i] = 0;
  }

  if (mype == 0) {
    for (int tpe = 1; tpe < npes; tpe++) {
      shmem_put_signal_nbi(dest, source, SIZE, &sig_addr, 1, SHMEM_SIGNAL_SET, tpe);
      shmem_local_complete();
      count++;
      for (int i = 0; i < SIZE; i++) {
        source[i] = count;
      }
    }
  } else {
    shmem_wait_until(&sig_addr, SHMEM_CMP_EQ, 1);
    for (int i = 0; i < SIZE; i++) {
      if (dest[i] != mype) {
        count++;
      }
    }
    if (count) fprintf(stderr, "Program Error\n");
  }
  shmem_finalize();
  return 0;
}
