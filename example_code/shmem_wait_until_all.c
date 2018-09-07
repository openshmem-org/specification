#include <shmem.h>

int main(void)
{
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int *flags = shmem_calloc(npes, sizeof(int));

  for (int i = 0; i < npes; i++)
      shmem_p(&flags[mype], 1, i);
  
  shmem_wait_until_all(flags, npes, SHMEM_CMP_EQ, 1);

  shmem_free(flags);
  shmem_finalize();
  return 0;
}
