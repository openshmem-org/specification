#include <shmem.h>
#include <stdio.h>
main()
{
    static long pSync[SHMEM_ALLTOALL_SYNC_SIZE];
    long *source, *dest;
    int  i, count, pe;
    
    start_pes(0);
    
    count = 2;
    dest = (long*) shmalloc(count * shmem_n_pes() * sizeof(long));
    source = (long*) shmalloc(count * shmem_n_pes() * sizeof(long));
    
    /* assign source values */
    for (pe=0; pe <shmem_n_pes(); pe++){
       for (i=0; i<count; i++){
          source[*pe*count)+i] = shmem_my_pe() + pe;
          dest[(pe*count)+i] = 9999;
       }
    }
    
    for (i=0; i< SHMEM_ALLTOALL_SYNC_SIZE; i++) {
       pSync[i] = SHMEM_SYNC_VALUE;
    }
    /* wait for all PEs to initialize pSync */
    shmem_barrier_all();
    
    /* alltoalls on all PES */
    shmem_alltoalls(dest, source, 1, 1, count * sizeof(long), 0, 0,
                    shmem_n_pes(), pSync);
    
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
    shfree(dest);
    shfree(source);
    shmem_finalize();
}
