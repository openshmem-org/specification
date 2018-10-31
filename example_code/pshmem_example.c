#include <stdio.h>
#include <sys/time.h>
#include <pshmem.h>

static double total_put_time = 0.0;
static double avg_put_time = 0.0;
static long put_count = 0;

static inline double get_wtime(void) {
    double wtime = 0.0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    wtime = tv.tv_sec;
    wtime += (double)tv.tv_usec / 1.0e6;
    return wtime;
}

void shmem_long_put(long *target, const long *source, size_t nelems, int pe)
{
    double t_start = get_wtime();                                             /* Start timer */
    pshmem_long_put(target, source, nelems, pe);                 /* Name shifted call to put */
    total_put_time += get_wtime() - t_start;                 /* Calculate total time elapsed */
    put_count += 1;                                                  /* Increment put counts */
    avg_put_time = total_put_time / (double) put_count;     /* Calculate average put latency */

    return;
}
