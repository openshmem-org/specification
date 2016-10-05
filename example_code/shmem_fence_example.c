#include <stdio.h>
#include <shmem.h>

long dest[10] = {0};
int targ = 0;

int main(void)
{
  long source[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  int src = 99;
  shmem_init();
  if (shmem_my_pe() == 0) {
    shmem_long_put(dest, source, 10, 1);  /*put1*/
    shmem_long_put(dest, source, 10, 2);  /*put2*/
    shmem_fence();
    shmem_int_put(&targ, &src, 1, 1);  /*put3*/
    shmem_int_put(&targ, &src, 1, 2);  /*put4*/
  }
  shmem_barrier_all();  /* sync sender and receiver */
  printf("dest[0] on PE %d is %ld\n", shmem_my_pe(), dest[0]);
  shmem_finalize();
  return 0;
}
