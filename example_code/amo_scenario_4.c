#include <shmem.h>

int main(void) {
  static int x = 0;

  shmem_init();
  /* Undefined behavior: OpenSHMEM atomic increment operations are concurrent
   * with the local increment of symmetric variable 'x'. */
  if (shmem_my_pe() > 0)
    shmem_int_atomic_inc(&x, 0);
  else
    x++;

  shmem_finalize();
  return 0;
}
