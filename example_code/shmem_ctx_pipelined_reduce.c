#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#define LEN 8192 /* Full buffer length */
#define PLEN 512 /* Length of each pipeline stage */

int in_buf[LEN], out_buf[LEN];

int main(void) {
  int i, j, *pbuf[2];
  shmem_ctx_t ctx[2];

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  pbuf[0] = shmem_malloc(PLEN * npes * sizeof(int));
  pbuf[1] = shmem_malloc(PLEN * npes * sizeof(int));

  int ret_0 = shmem_ctx_create(0, &ctx[0]);
  int ret_1 = shmem_ctx_create(0, &ctx[1]);
  if (ret_0 || ret_1)
    shmem_global_exit(1);

  for (i = 0; i < LEN; i++) {
    in_buf[i] = mype;
    out_buf[i] = 0;
  }

  int p_idx = 0, p = 0; /* Index of ctx and pbuf (p_idx) for current pipeline stage (p) */
  for (i = 1; i <= npes; i++)
    shmem_put_nbi(ctx[p_idx], &pbuf[p_idx][PLEN * mype], &in_buf[PLEN * p], PLEN,
                  (mype + i) % npes);

  /* Issue communication for pipeline stage p, then accumulate results for stage
   * p-1 */
  for (p = 1; p < LEN / PLEN; p++) {
    p_idx ^= 1;
    for (i = 1; i <= npes; i++)
      shmem_put_nbi(ctx[p_idx], &pbuf[p_idx][PLEN * mype], &in_buf[PLEN * p], PLEN,
                    (mype + i) % npes);

    shmem_ctx_quiet(ctx[p_idx ^ 1]);
    shmem_sync_all();
    for (i = 0; i < npes; i++)
      for (j = 0; j < PLEN; j++)
        out_buf[PLEN * (p - 1) + j] += pbuf[p_idx ^ 1][PLEN * i + j];
  }

  shmem_ctx_quiet(ctx[p_idx]);
  shmem_sync_all();
  for (i = 0; i < npes; i++)
    for (j = 0; j < PLEN; j++)
      out_buf[PLEN * (p - 1) + j] += pbuf[p_idx][PLEN * i + j];

  shmem_finalize();
  return 0;
}
