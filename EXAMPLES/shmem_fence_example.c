#include <stdio.h>
#include <shmem.h>

long target[10];
int targ;

int main(void)
{
   long source[10] = { 1, 2, 3, 4, 5,
                       6, 7, 8, 9, 10 };
   int src;
   start_pes(0);
   if (_my_pe() == 0) {
      shmem_long_put(target, source, 10, 1);  /*put1*/
      shmem_long_put(target, source, 10, 2);  /*put2*/
      shmem_fence();
      shmem_int_put(targ, src, 1, 1);  /*put3*/
      shmem_int_put(targ, src, 1, 2);  /*put4*/
      }
      shmem_barrier_all();  /* sync sender and receiver */
      printf("target[0] on PE %d is %d\n", _my_pe(), target[0]);
      return 1;
}

