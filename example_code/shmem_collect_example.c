#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  static long lock = 0;
  static long pSync[SHMEM_COLLECT_SYNC_SIZE];
  for (int i = 0; i < SHMEM_COLLECT_SYNC_SIZE; i++)
    pSync[i] = SHMEM_SYNC_VALUE;

  shmem_init();
  int me = shmem_my_pe();
  int npes = shmem_n_pes();
  int my_nelem = me + 1; /* linearly increasing number of elements with PE */
  int total_nelem = (npes * (npes + 1)) / 2;

  int *source = (int *)shmem_malloc(npes * sizeof(int)); /* symmetric alloc */
  int *dest = (int *)shmem_malloc(total_nelem * sizeof(int));

  for (int i = 0; i < my_nelem; i++)
    source[i] = (me * (me + 1)) / 2 + i;
  for (int i = 0; i < total_nelem; i++)
    dest[i] = -9999;

  shmem_barrier_all(); /* Wait for all PEs to update source/dest */

  shmem_collect32(dest, source, my_nelem, 0, 0, npes, pSync);

  shmem_set_lock(&lock); /* Lock prevents interleaving printfs */
  printf("%d: %d", me, dest[0]);
  for (int i = 1; i < total_nelem; i++)
    printf(", %d", dest[i]);
  printf("\n");
  shmem_clear_lock(&lock);
  shmem_finalize();
  return 0;
}
