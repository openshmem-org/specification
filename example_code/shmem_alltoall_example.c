#include <shmem.h>
#include <stdio.h>

long pSync[SHMEM_ALLTOALL_SYNC_SIZE];

int main(void)
{
    long *source, *dest;
    int  i, count, pe;
    
    shmem_init();
    
    count = 2;
    dest = (long*) shmem_malloc(count * shmem_n_pes() * sizeof(long));
    source = (long*) shmem_malloc(count * shmem_n_pes() * sizeof(long));
    
    /* assign source values */
    for (pe=0; pe <shmem_n_pes(); pe++){
       for (i=0; i<count; i++){
          source[pe*count)+i] = shmem_my_pe() + pe;
          dest[(pe*count)+i] = 9999;
       }
    }
    
    for (i=0; i< SHMEM_ALLTOALL_SYNC_SIZE; i++) {
       pSync[i] = SHMEM_SYNC_VALUE;
    }
    /* wait for all PEs to initialize pSync */
    shmem_barrier_all();
    
    /* alltoall on all PES */
    shmem_alltoall64(dest, source, count, 0, 0, shmem_n_pes(), pSync);
    
    /* verify results */
    for (pe=0; pe<shmem_n_pes(); pe++) {
       for (i=0; i<count; i++){
          if (dest[(pe*count)+i] != shmem_my_pe() + pe) {
          printf("[%d] ERROR: dest[%d]=%ld, should be %d\n",
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
