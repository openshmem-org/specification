#include <stdio.h>
#include <shmem.h>
int dst;

int main ()
{
   int me;

   start_pes (0);
   me = _my_pe ();

   dst = 74;
   shmem_barrier_all ();

   if (me == 0)
      shmem_int_inc (&dst, 1);
   shmem_barrier_all ();

   printf ("%d: dst = %d\n", me, dst);
   return 0;
}
