#include <shmem.h>
#include <stdio.h>

int user_wait_any(long *ivar, int count, int cmp, long value) {
  int idx = 0;
  while (!shmem_test(&ivar[idx], cmp, value))
    idx = (idx + 1) % count;
  return idx;
}

int main(void) {
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();

  long *wait_vars = shmem_calloc(npes, sizeof(long));
  if (mype == 0) {
    int who = user_wait_any(wait_vars, npes, SHMEM_CMP_NE, 0);
    printf("PE %d observed first update from PE %d\n", mype, who);
  }
  else
    shmem_atomic_set(&wait_vars[mype], mype, 0);

  shmem_free(wait_vars);
  shmem_finalize();
  return 0;
}
