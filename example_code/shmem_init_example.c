#include <stdio.h>
#include <shmem.h>

int main(void) {
    static int targ = 0;

    shmem_init();
    int me = shmem_my_pe();
    int receiver = 1 % shmem_n_pes();

    if (me == 0) {
        int src = 33;
        shmem_put(&targ, &src, 1, receiver);
    }

    shmem_barrier_all(); /* Synchronizes sender and receiver */

    if (me == receiver)
        printf("PE %d targ=%d (expect 33)\n", me, targ);

    shmem_finalize();
    return 0;
}
