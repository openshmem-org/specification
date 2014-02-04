#include <stdio.h>
#include <shmem.h>
int dst;

int main ()
{
   int me;
   int old;

   start_pes (0);
   me = _my_pe ();

   old = -1;
   dst = 22;
   shmem_barrier_all ();

   if (me == 0)
      old = shmem_int_finc (&dst, 1);

   shmem_barrier_all ();
   printf ("%d: old = %d, dst = %d\n", me, old, dst);
   return 0;
}
