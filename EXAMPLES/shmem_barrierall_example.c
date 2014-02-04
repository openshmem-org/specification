#include <stdio.h>
#include <shmem.h>
int x=1010;

int main ()
{
   int me, npes;

   start_pes (0);
   me = _my_pe ();
   npes = _num_pes ();

   /*put to next  PE in a circular fashion*/
   shmem_int_p (&x, 4, me+1%npes);
   /*synchronize all PEs*/
   shmem_barrier_all ();

   printf ("%d: x = %d\n", me, x);
   return 0;
}
