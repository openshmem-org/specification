#include <stdio.h>
#include <inttypes.h>
#include <shmem.h>

long pSync[SHMEM_ALLTOALLS_SYNC_SIZE];

int main(void)
{
    int64_t *source, *dest;
    int  i, count, pe;
    
    shmem_init();
    
    count = 2;
    dest = (int64_t*) shmem_malloc(count * shmem_n_pes() * sizeof(int64_t));
    source = (int64_t*) shmem_malloc(count * shmem_n_pes() * sizeof(int64_t));
    
    /* assign source values */
    for (pe=0; pe <shmem_n_pes(); pe++){
       for (i=0; i<count; i++){
          source[(pe*count)+i] = shmem_my_pe() + pe;
          dest[(pe*count)+i] = 9999;
       }
    }
    
    for (i=0; i< SHMEM_ALLTOALLS_SYNC_SIZE; i++) {
       pSync[i] = SHMEM_SYNC_VALUE;
    }
    /* wait for all PEs to initialize pSync */
    shmem_barrier_all();
    
    /* alltoalls on all PES */
    shmem_alltoalls64(dest, source, 1, 1, count, 0, 0, shmem_n_pes(), pSync);
    
    /* verify results */
    for (pe=0; pe<shmem_n_pes(); pe++) {
       for (i=0; i<count; i++){
          if (dest[(pe*count)+i] != shmem_my_pe() + pe) {
          printf("[%d] ERROR: dest[%d]=%" PRId64 ", should be %d\n",
                  shmem_my_pe(),(pe*count)+i,dest[(pe*count)+i],
                  shmem_n_pes() + pe);
            }
        }
    }
    
    shmem_barrier_all();
    shmem_free(dest);
    shmem_free(source);
    shmem_finalize();
    return 0;
}
