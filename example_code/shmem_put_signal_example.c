#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shmem.h>

#define ITERATIONS (100)
#define MAX_SIZE   (2<<18)

int
main(int argc, char* argv[])
{
    shmem_init();

    int       me      = shmem_my_pe();
    int       n       = shmem_n_pes();
    int       r       = ITERATIONS;
    size_t    bloat   = MAX_SIZE;
    size_t    size;

    for (size = 1; size < bloat; size*=2) {
        uint64_t* message = malloc(size * sizeof(uint64_t));
        uint64_t* data    = shmem_malloc(r * size * sizeof(uint64_t));
        uint64_t* signals = shmem_malloc(r * sizeof(uint64_t));

        memset(message, 0, size * sizeof(uint64_t));
        memset(data, 0, r * size * sizeof(uint64_t));
        memset(signals, 0, r * sizeof(uint64_t));
        shmem_barrier_all();

        message[0] = 10;
        int i;
        for (i = 0; i < r; i++) {
            int j = i - (me == 0);
            if (j >= 0) {
                shmem_long_wait_until((long *)&signals[j],
                                      SHMEM_CMP_EQ, 1);
                message[0] = data[j * size] + 10;
            }
            int pe = (me + 1) % n;
            shmemx_putmem_signal(&data[i * size], message,
                                size * sizeof(uint64_t),
                                &signals[i], 1, pe);
        }
        if (me == 0) {
            shmem_long_wait_until((long *)&signals[r-1],
                                  SHMEM_CMP_EQ, 1);
            printf("Final message = %lu for size %zu\n",
                    data[(r-1) * size], size);
        }

        shmem_barrier_all();
        shmem_free(signals);
        shmem_free(data);
        free(message);
        shmem_barrier_all();
    }

    shmem_finalize();
  return 0;
}
