#include <stdio.h>
#include <shmem.h>

long target[3] = {0};
int targ = 0;
long source[3] = {1, 2, 3};
int src = 90;

int main(void)
{
  long x[3] = {0};
  int y = 0;
  
  start_pes(0);
  if (_my_pe() == 0) {
    shmem_long_put(target, source, 3, 1);  /*put1*/
    shmem_int_put(&targ, &src, 1, 2);  /*put2*/

    shmem_quiet();

    shmem_long_get(x, target, 3, 1);    /*gets updated value from target on PE 1 to local array x */
    shmem_int_get(&y, &targ, 1, 2);     /*gets updated value from targ on PE 2 to local variable y*/
    printf("x: {%ld,%ld,%ld}\n",x[0],x[1],x[2]); /*x: {1,2,3}*/
    printf("y: %d\n", y); /*y: 90*/

    shmem_int_put(&targ, &src, 1, 1);  /*put3*/
    shmem_int_put(&targ, &src, 1, 2);  /*put4*/
  }
  shmem_barrier_all();  /* sync sender and receiver */
  return 0;
}

