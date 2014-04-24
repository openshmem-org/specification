#include <stdio.h>
#include <shmem.h>

long target[3] = {0};
int targ = 0;
long source[3] = {1, 2, 3};
int src = 90;

int main(void)
{
  start_pes(0);
  if (_my_pe() == 0) {
    shmem_long_put(target, source, 3, 1);  /*put1*/
    shmem_int_put(&targ, &src, 1, 2);  /*put2*/

    shmem_quiet();

    shmem_long_get(target, target, 3, 1);    /*gets updated value from target on PE 1*/
    shmem_int_get(&targ, &targ, 1, 2);       /*gets updated value from targ on PE 2*/
    printf("target: {%ld,%ld,%ld}\n",target[0],target[1],target[2]); /*target: {1,2,3}*/
    printf("targ: %d\n", targ); /*targ: 90*/

    shmem_int_put(&targ, &src, 1, 1);  /*put3*/
    shmem_int_put(&targ, &src, 1, 2);  /*put4*/
  }
  shmem_barrier_all();  /* sync sender and receiver */
  printf("target[0] on PE %d is %ld\n", _my_pe(), target[0]);
  return 0;
}

