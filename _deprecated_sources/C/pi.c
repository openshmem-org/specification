/*
 * This file is distributed as part of GatorSHMEM, a project of the HCS
 * Research Lab / CHREC at the University of Florida.
 *
 * Copyright (c) 2005-2010, the University of Florida.
 * All rights reserved.
 *
 * Modified by SPoole from ORNL to be OpenSHMEM V1.0 compliant
 * and work with other architectures.
 *
 */
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <shmem.h>

#define M_PI_2     1.57079632679489661923
#define TRIES      1000000000


double timerval()
{
        struct timeval st;
        gettimeofday ( &st, NULL);
        return st.tv_sec + st.tv_usec * 1e-6;
}


int main(int argc, char *argv[])
{
/* 1. get random [0, 1] ==>
   2. get random [0, pi/2] ==>theta
   3. hit X < sin(theta)
   4. 2/pi = hit/tries.
*/
   double X, Theta, My_pi;
   double Tstart, Tend;
   int i, total, hit=0;
   int *buf, my_mem;
   int rank, numprocs, num_of_procs;
 
   num_of_procs = atoi(argv[1]);

   start_pes (0);

   numprocs = _num_pes();
   rank     = _my_pe();

   my_mem = (sizeof(int) * numprocs);
   buf = shmalloc(my_mem);

   srand( (unsigned int) time(NULL));

   if ( rank == 0 ) {
      printf("pi is %f\n", M_PI_2 );
      printf("sin(pi/2) is %f\n", sin(M_PI_2));
      fflush(stdout);
   }

   Tstart = timerval();

   if ( rank != 0 ) {
      total = TRIES/(numprocs-1);
         if (rank == 1)
            total += TRIES % (numprocs-1);

    srand( (unsigned int) time(NULL));

    for ( i = 0; i < total ; i++){
    X = rand();
    X = X/RAND_MAX;

    Theta = rand();
    Theta = ( M_PI_2 ) * (Theta/RAND_MAX);
    if ( X < sin(Theta))
       hit++;
    }
   buf[0] = hit;
   }

   shmem_barrier_all();

   if ( rank == 0 )
      for ( i = 1; i < numprocs; i++) {
         shmem_getmem(buf, buf, sizeof(int), i);
         hit += buf[0];
         printf("from node(%d), getmem buf\t %d, so hit is\t %d\n", i, buf[0], hit);
      }

   shmem_barrier_all();

   if ( rank == 0 ) {
   My_pi = 2 * ( 1 / ( ((double)hit)/TRIES ) );
   Tend = timerval();
   printf("Hit is : %d :: Total is %d \n",hit, TRIES);
   printf("My pi is %.16f \n", My_pi);
   printf("Elapsed time is %f \n", Tend - Tstart);
   }

   shmem_barrier_all();

   return 0;
}
