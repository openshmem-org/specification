#include <math.h>
#include <shmem.h>
#include <stdio.h>

int main(void) {
  const double e = 2.71828182;
  const double epsilon = 0.00000001;
  static double f = 3.1415927;
  shmem_init();
  int mype = shmem_my_pe();
  if (mype == 0)
    shmem_p(&f, e, 1);
  shmem_barrier_all();
  if (mype == 1)
    printf("%s\n", (fabs(f - e) < epsilon) ? "OK" : "FAIL");
  shmem_finalize();
  return 0;
}
