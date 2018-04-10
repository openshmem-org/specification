#include <stdio.h>
#include <shmem.h>
#include <assert.h>
#include <string.h>


void user_wait_all(int *ivars, size_t nelems, size_t *indices, int cmp, int value)
{
  size_t count = 0;
  while (count < nelems)
    count = shmem_wait_until_some(ivars, nelems, indices, cmp, value);
  return;
}

int main(void)
{
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int *ivars = shmem_calloc(npes, sizeof(int));
  size_t *indices = (size_t *)malloc(npes * sizeof(size_t));

  for (int i=0; i<npes; i++)
    indices[i] = nelems;

  if (mype == 0) {
    user_wait_all(ivars, npes, indices, SHMEM_CMP_NE, 0);
    for (int i=0; i<npes; i++)
      assert(ivars[i] == i);
  }
  else
    shmem_p(&ivars[mype], mype, 0);

  free(indices);
  shmem_free(ivars);
  shmem_finalize();
  return 0;
}
