#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int          status = 0; 
  static long  source;
  long        *dest;
  shmem_req_h *requests;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  dest     = (long *)shmem_malloc(npes * sizeof(long));
  requests = (shmem_req_h *) malloc(npes * sizeof(shmem_req_h));
  for (int i = 0; i < npes; i++) {
    requests[i] = SHMEM_REQ_INVALID;
  }

  source = mype;
  for (int i = 0; i < npes; i++) {
    status = shmem_broadcast_nb(SHMEM_TEAM_WORLD, &dest[i], source,
                                1, i, &requests[i]);
    if (0 != status) {
      fprintf(stderr, "shmem broadcast nb failed with root %d and status %d\n",
                       i, status);
      goto out;
    }
  }

  for (int i = 0; i < npes; i++) {
    status = shmem_req_wait(&requests[i]);
    if (0 != status) {
      fprintf(stderr, "shmem req wait failed on request %d\n", i);
      goto out;
    }
  }

  if (mype == 0) {
    for (int i = 0; i < npes; i++) {
      if (i > 0 && !(i % 8)) {
        printf("\n");
      }
      printf("%8d", dest[i]);
    }
    printf("\n");
  }
out:
  shmem_free(dest);
  free(requests);
  shmem_finalize();
  return status;
}
