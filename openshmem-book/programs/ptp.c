/* Passive Target Progress test */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>

#include <shmem.h>

#ifdef __GNUC__
int pe_escape = 1;
#define mb() __sync_synchronize()
#else
/* this isn't entirely right, may need to fill in your compiler or
   platform's memory barrier */
volatile int pe_escape = 1;
#endif

int A = 0;

int
main(int argc, char **argv)
{
     int me, npes;
     struct timeval now;
     long t_start, t_end;

     start_pes(0);
     me = _my_pe();
     npes = _num_pes();

     if (npes < 4) {
          if (me==0)
               fprintf(stderr,"ERR: test requires 4 or more PEs\n");
          return 1;
     }
     shmem_barrier_all();

     gettimeofday(&now, NULL);
     t_start = (now.tv_sec * 1000000.0) + now.tv_usec;

     switch (me) {
     case 0:
          while (pe_escape) {
               double pi, pi2, pi3;
               int j;

               for (j=1; j <= 5000; j++) {
                    pi = (22.0 / 7.0) + (double) j;
                    pi2 = pi * (double) j;
                    pi3 = (pi2 * pi) / 1.2;
               }
               mb();
          }
          gettimeofday(&now, NULL);
          t_end = ((now.tv_sec * 1000000.0) + now.tv_usec) - t_start;
          break;

     case 1:
          shmem_int_inc(&A, 0);
          gettimeofday(&now, NULL);
          t_end = ((now.tv_sec * 1000000.0) + now.tv_usec) - t_start;
          break;

     case 2:
          while (1 != shmem_int_g(&A, 0)) { ; }
          shmem_int_inc(&A, 0);
          gettimeofday(&now, NULL);
          t_end = ((now.tv_sec * 1000000.0) + now.tv_usec) - t_start;
          break;

     case 3:
          while (2 != shmem_int_g(&A, 0)) { ; }
          shmem_int_p((int*) &pe_escape, 0, 0);  // release PE0.
          if (npes > 4) {
               int i;

               for(i=4; i < npes; i++)
                    shmem_int_p((int*)&pe_escape, 0, i);  // release PE0.
          }
          gettimeofday(&now, NULL);
          t_end = ((now.tv_sec * 1000000.0) + now.tv_usec) - t_start;
          break;

     default:
          /* spin until released, A will never == 99, generate PE-0 traffic */
          while (99 != shmem_int_g(&A, 0) && pe_escape) {
               mb();
          }
          gettimeofday(&now, NULL);
          t_end = ((now.tv_sec * 1000000.0) + now.tv_usec) - t_start;
          break;
     }

     if (me < 4)
          fprintf(stderr,"[%d] elapsed usecs %ld A %d\n",me,t_end,A);

     shmem_barrier_all();

     return 0;
}
