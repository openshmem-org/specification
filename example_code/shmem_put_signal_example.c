#include <stdio.h>
#include <shmem.h>
#include <stdlib.h>

int main(void)
{
    int i, err_count  = 0;

    shmem_init();

    size_t    size    = (2<<10);
    int       me      = shmem_my_pe();
    int       n       = shmem_n_pes();
    int       pe      = (me + 1)%n;
    uint64_t* message = malloc(size * sizeof(uint64_t));
    uint64_t* data    = shmem_malloc(size * sizeof(uint64_t));
    static uint64_t sig_addr = 0;

    for (i = 0; i < size; i++) {
        message[i] = me;
        data[i]    = 0;
    }
    shmem_barrier_all();

    if (me != 0) {
        shmem_uint64_wait_until(&sig_addr, SHMEM_CMP_EQ, 1);
    }

    shmem_putmem_signal(data, message, size*sizeof(uint64_t),
            &sig_addr, 1, pe);

    if (me == 0) {
        shmem_uint64_wait_until(&sig_addr, SHMEM_CMP_EQ, 1);
        printf("BCAST with put with signal is complete\n");
    }

    free(message);
    shmem_free(data);

    shmem_finalize();
    return 0;
}