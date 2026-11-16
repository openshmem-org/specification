#include <inttypes.h>
#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int          status = 0;
  const int    count  = 2;
  const int    nr_a2a = 2;
  int64_t    **source;
  int64_t    **dest;
  shmem_req_h *requests;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  source   = (int64_t **)shmem_malloc(nr_a2a * sizeof(int64_t *));
  dest     = (int64_t **)shmem_malloc(nr_a2a * sizeof(int64_t *));
  requests = (shmem_req_h *)malloc(nr_a2a * sizeof(shmem_req_h));
  for (int i = 0; i < nr_a2a; i++) {
    requests[i] = SHMEM_REQ_INVALID;
  }

  for (int nr = 0; nr < nr_a2a; nr++) {
    dest[nr]   = (int64_t *)shmem_malloc(count * npes * sizeof(int64_t));
    source[nr] = (int64_t *)shmem_malloc(count * npes * sizeof(int64_t));
    for (int pe = 0; pe < npes; pe++) {
      for (int i = 0; i < count; i++) {
        source[nr][(pe * count) + i] = mype + pe;
        dest[nr][(pe * count) + i]   = 9999;
      }
    }
  }

  /* wait for all PEs to update sources/dests */ 
  shmem_team_sync(SHMEM_TEAM_WORLD);

  /* concurrent alltoall operations on all PEs */
  for (int i = 0; i < nr_a2a; i++) {
    status = shmem_int64_alltoall_nb(SHMEM_TEAM_WORLD, dest[i], source[i], count, &requests[i]);
    if (0 != status) {
      fprintf(stderr, "shmem alltoall nb failed with status %d\n", status);
      goto out;
    }
  }

  for (int i = 0; i < nr_a2a; i++) {
    status = shmem_req_wait(&requests[i]);
    if (0 != status) {
      fprintf(stderr, "shmem req wait failed on request %d\n", i);
      goto out;
    }
  }

  for (int nr = 0; nr < nr_a2a; nr++) {
    for (int pe = 0; pe < npes; pe++) {
      for (int i = 0; i < count; i++) {
        if (dest[nr][(pe * count) + i] != pe + mype) {
          printf("[%d] ERROR: dest[%d]=%" PRId64 ", should be %d\n",
            mype, (pe * count) + i, dest[nr][(pe * count) + i], pe + mype);
        }
      }
    }
  }

out:
  for (int nr = 0; nr < nr_a2a; nr++) {
    shmem_free(source[nr]);
    shmem_free(dest[nr]);
  }
  shmem_free(source);
  shmem_free(dest);
  free(requests);
  shmem_finalize();
  return status;
}
