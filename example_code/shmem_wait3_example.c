#include <stdio.h>
#include <shmem.h>

int ivar;

void wait_on_ivar(void) {
    shmem_int_wait_until(&ivar, SHMEM_CMP_LT, 0);
}
