#include <shmem.h>
#include <stdio.h>

int main(void) {
  int src = 99;
  long source[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  static long dest[10];
  static int targ;
  shmem_init();
  int me = shmem_my_pe();
  if (me == 0) {
    shmem_put(dest, source, 10, 1); /* put1 */
    shmem_put(dest, source, 10, 2); /* put2 */
    shmem_fence();
    shmem_put(&targ, &src, 1, 1); /* put3 */
    shmem_put(&targ, &src, 1, 2); /* put4 */
  }
  shmem_barrier_all(); /* sync sender and receiver */
  printf("dest[0] on PE %d is %ld\n", me, dest[0]);
  shmem_finalize();
  return 0;
}
