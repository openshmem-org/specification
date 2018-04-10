#include <stdio.h>
#include <shmem.h>

int main(void)
{
  shmem_init();
  const int mype = shmem_my_pe();
  const int npes = shmem_n_pes();

  int *ivars = shmem_calloc(npes, sizeof(int));
  size_t *indices = (size_t *)malloc(npes * sizeof(size_t));

  if (mype == 0)
  {
    size_t count = shmem_test_some(ivars, nelems, indices, SHMEM_CMP_NE, 0);
    printf("PE %d observed %zu puts\n", mype, count);
  }
  else
    shmem_p(&ivars[mype], mype, 0);

  free(indices);
  shmem_free(ivars);
  shmem_finalize();
  return 0;
}
