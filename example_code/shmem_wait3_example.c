#include <shmem.h>
#include <stdio.h>

int ivar;
shmem_int_wait_until(&ivar, SHMEM_CMP_LT, 0);
