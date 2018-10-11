#include <shmem.h>
#include <stdlib.h>
 
int main(void)
{
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
 
  int *flags = shmem_calloc(npes, sizeof(int));
  int *status = calloc(npes, sizeof(int));
  int *processed = calloc(npes, sizeof(int));
 
  for (int i = 0; i < npes; i++)
      shmem_p(&flags[mype], 1, i);
 
  int ncompleted = 0, completed_idx;
 
  while (ncompleted < npes) {
      completed_idx = shmem_test_any(flags, npes, status, SHMEM_CMP_EQ, 1);
      if (completed_idx != SIZE_MAX && !processed[completed_idx]) {
          ncompleted++;
          processed[completed_idx] = 1;
      } else {
          /* Overlap some computation here */
      }
  }
 
  free(status);
  shmem_free(flags);
  shmem_finalize();
  return 0;
}
