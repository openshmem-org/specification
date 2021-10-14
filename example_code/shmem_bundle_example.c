#include <shmem.h>
#include <stdlib.h>

#define N_UPDATES (1 << 20)
#define N_INDICES (1 << 10)
#define N_VALUES  (1 << 31)

int main(void) {

  shmem_init();

  uint64_t *table = shmem_malloc(N_INDICES * sizeof(uint64_t));

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  shmem_bundle_start();
  for (size_t i = 0; i < N_UPDATES; i++) {
      int random_pe = rand() % npes;
      size_t random_idx = rand() % N_INDICES;
      uint64_t random_val = rand() % N_VALUES;
      shmem_uint64_atomic_xor(&table[random_idx], random_val, random_pe);
  }
  shmem_bundle_stop();

  shmem_free(table);
  shmem_finalize();
  return 0;
}
