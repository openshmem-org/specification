#include <stdio.h>
#include <shmemx.h>
#include <pshmem.h>

static double totalTimeForPut = 0.0;
static double avgTimeForPut = 0.0;
static long countForPut = 0;

void shmem_long_put(long *target, const long *source, size_t nelems, int pe)
{
    double tstart = shmemx_wtime();                                           /* Start timer */
    pshmem_long_put(target, source, nelems, pe);                 /* Name shifted call to put */
    totalTimeForPut += shmemx_wtime() - tstart;              /* Calculate total time elapsed */
    countForPut += 1;                                                /* Increment put counts */
    avgTimeForPut = totalTimeForPut / (double) countForPut; /* Calculate average put latency */

    return;
}
