#include <stdio.h>
#include <shmem.h>
#include <assert.h>
#include <string.h>


void user_wait_all(int *ivars, size_t nelems, _Bool *status, int cmp, int value)
{
  size_t count = 0;
  while (count = shmem_wait_until_some(ivars, nelems, status, cmp, value));
  return;
}

int main(void)
{
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int *ivars = shmem_calloc(npes, sizeof(int));
  _Bool *status = malloc(npes * sizeof(_Bool));

  for (int i=0; i<npes; i++)
    status[i] = 0;

  if (mype == 0) {
    user_wait_all(ivars, npes, status, SHMEM_CMP_NE, 0);
    for (int i=0; i<npes; i++)
      assert(ivars[i] == i);
  }
  else
    shmem_p(&ivars[mype], mype, 0);

  free(status);
  shmem_free(ivars);
  shmem_finalize();
  return 0;
}
