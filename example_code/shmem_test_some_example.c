#include <stdio.h>
#include <shmem.h>

int main(void)
{
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();

  int *ivars = shmem_calloc(npes, sizeof(int));
  _Bool *status = calloc(npes, sizeof(_Bool));

  if (mype == 0)
  {
    size_t count = shmem_test_some(ivars, nelems, status, SHMEM_CMP_NE, 0);
    printf("PE 0 observed %zu puts\n", count);
  }
  else
    shmem_p(&ivars[mype], mype, 0);

  free(status);
  shmem_free(ivars);
  shmem_finalize();
  return 0;
}
