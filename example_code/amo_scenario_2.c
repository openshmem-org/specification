#include <stdint.h>
#include <shmem.h>

int main(void) {
  static uint64_t x = 0;

  shmem_init();
  /* Undefined behavior: The following AMOs access the same location
   * concurrently using different types. */
  if (shmem_my_pe() > 0)
    shmem_uint32_atomic_or((uint32_t *)&x, shmem_my_pe() + 1, 0);
  else
    shmem_uint64_atomic_or(&x, shmem_my_pe() + 1, 0);

  shmem_finalize();
  return 0;
}
