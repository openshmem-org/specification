#include <shmem.h>
#include <stdio.h>

int main(void) {
  static long dest[3];
  static long source[3] = {1, 2, 3};
  static int targ;
  static int src = 90;
  long x[3] = {0};
  int y = 0;
  shmem_init();
  int mype = shmem_my_pe();
  if (mype == 0) {
    shmem_put(dest, source, 3, 1); /* put1 */
    shmem_put(&targ, &src, 1, 2);  /* put2 */
    shmem_quiet();
    shmem_get(x, dest, 3, 1); /* gets updated value from dest on PE 1 to local array x */
    shmem_get(&y, &targ, 1,
              2); /* gets updated value from targ on PE 2 to local variable y */
    printf("x: { %ld, %ld, %ld }\n", x[0], x[1], x[2]); /* x: { 1, 2, 3 } */
    printf("y: %d\n", y);                               /* y: 90 */
    shmem_put(&targ, &src, 1, 1);                       /* put3 */
    shmem_put(&targ, &src, 1, 2);                       /* put4 */
  }
  shmem_finalize();
  return 0;
}
