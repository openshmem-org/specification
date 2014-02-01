#include "bench.h"
#include <shmem.h>

#define PERM(ME,TOT,ITER)  ((ME)^(ITER))         // ok if 2^n pes

#define MAX(A,B)  (((A)>(B)) ? (A) : (B))
#define MIN(A,B)  (((A)<(B)) ? (A) : (B))

int64  do_all2all      (uint64 *dst, uint64 *src, int64 len, int64 nwrd);
int64  accum_long      (int64 val);

extern int64 SELF, SIZE;

/* returns words sent per PE */

int64  do_all2all      (uint64 *dst, uint64 *src, int64 len, int64 nwrd)
{
static char cvs_info[] = "BMKGRP $Date: $ $Revision: $ $RCSfile: all2all.c,v $ $Name: $";

  int64 i, j, pe;

  len = len - (len % (nwrd * SIZE));  // force even multiple
  for (i = 0; i < len; i+=SIZE*nwrd) {
    shmem_barrier_all();
    for (j = 0; j < SIZE; j++) {
      pe = PERM(SELF,SIZE,j);
      /* shmem_put (&dst[i + SELF*nwrd], &src[i + pe*nwrd], nwrd, pe);*/
      shmem_put64 (&dst[i + SELF*nwrd], &src[i + pe*nwrd], nwrd, pe);
    }
  }
  return len;
}

int64  accum_long      (int64 val)

{
  int64 i;
  static int64 target, source, init=0;
  static int64 Sync[_SHMEM_REDUCE_SYNC_SIZE];
  static int64 Work[2 + _SHMEM_REDUCE_MIN_WRKDATA_SIZE];

  if (! init) {
    /* need to initialize Sync first time around */
    for(i = 0; i < _SHMEM_REDUCE_SYNC_SIZE; i++)
      Sync[i] = _SHMEM_SYNC_VALUE;
    init = 1;
  }
  source = val;
  shmem_barrier_all();

  shmem_long_sum_to_all (&target, &source, 1, 0, 0, SIZE, Work, Sync);

  shmem_barrier_all();
  return target;
}

