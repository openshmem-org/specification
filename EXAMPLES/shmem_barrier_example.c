#include <stdio.h>
#include <shmem.h>

long pSync[_SHMEM_BARRIER_SYNC_SIZE];
int x = 10101;

int main ()
{
  int me, npes;

  for (int i = 0; i < _SHMEM_BARRIER_SYNC_SIZE; i += 1){
      pSync[i] = _SHMEM_SYNC_VALUE;
  }

  start_pes (0);
  me = _my_pe ();
  npes = _num_pes ();

  if (me % 2 == 0){
      x = 1000 + me;
      /*put to next even PE in a circular fashion*/
      shmem_int_p (&x, 4, me+2%npes);
      /*synchronize all even pes*/
      shmem_barrier (0, 1, (npes/2 + npes%2), pSync);
  }
  printf ("%d: x = %d\n", me, x);
  return 0;
}
