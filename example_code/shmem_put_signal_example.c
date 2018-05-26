#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#define MAX_SIZE (2<<10)
#define VAL_USED 10

int
main(int argc, char* argv[])
{
    int i, err_count  = 0;

    shmem_init();

    size_t    size    = MAX_SIZE;
    int       me      = shmem_my_pe();
    int       n       = shmem_n_pes();
    int       pe      = (me + 1)%n;

    uint64_t* message = malloc(size * sizeof(uint64_t));
    uint64_t* data    = shmem_malloc(size * sizeof(uint64_t));
    uint64_t* signals = shmem_malloc(sizeof(uint64_t));

    signals[0] = 0;
    for (i = 0; i < size; i++) {
        message[i] = VAL_USED;
        data[i]    = 0;
    }
    shmem_barrier_all();

    if (me != 0) {
        shmem_long_wait_until((long *)&signals[0], SHMEM_CMP_EQ, 1);
    }

    shmemx_putmem_signal(data, message, size*sizeof(uint64_t),
            &signals[0], 1, pe);

    if (me == 0) {
        shmem_long_wait_until((long *)&signals[0], SHMEM_CMP_EQ, 1);
        printf("BCAST with put with signal is complete\n");
    }

    free(message);
    shmem_free(data);
    shmem_free(signals);

    shmem_finalize();
    return 0;
}
