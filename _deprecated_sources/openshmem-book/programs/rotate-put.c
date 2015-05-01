#include <stdio.h>

#include <shmem.h>

int d;                          /* symmetric target variable */

int
main (int argc, char *argv[])
{
  int me, npes;
  int rn;

  start_pes (0);                /* Initialize this PE */

  me = _my_pe ();               /* I am this PE */
  npes = _num_pes ();           /* There are this many PEs in the program */

  rn = (me + 1) % npes;         /* find my right neighbor (wrap-around) */

  d = me;                       /* value to send */

  shmem_int_p (&d, d, rn);      /* send to right neighbor */

  shmem_barrier_all ();         /* everyone must have sent */

  printf ("%d/%d: d = %d\n", me, npes, d);

  return 0;
}
