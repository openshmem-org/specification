#include <shmem.h>
#include <stdio.h>

main()
{
    static long pSync[SHMEM_ALLTOALL_SYNC_SIZE];
    long *source, *dest;
    size_t *s_offsets, *s_sizes, max_offset, *t_sizes, offset;
    int  i, maxcount, pe, idx;
    
    start_pes(0);
    
    maxcount = shmem_n_pes();
    max_offset = maxcount * sizeof(long);
    dest = (long *) shmalloc(maxcount * shmem_n_pes() * sizeof(long));
    source = (long *) shmalloc(maxcount * shmem_n_pes() * sizeof(long));
    s_offsets = (size_t *) shmalloc(shmem_n_pes() * sizeof(size_t));
    s_sizes   = (size_t *) shmalloc(shmem_n_pes() * sizeof(size_t));
    t_sizes   = (size_t *) shmalloc(shmem_n_pes() * sizeof(size_t));
    
    s_offsets[0] = 0;
    for (pe=0; pe <shmem_n_pes(); pe++) {
    
       /* set source sizes, no need to set dest sizes */
       s_sizes[pe] = pe * sizeof(long);
       t_sizes[pe] = 0;
    
       /* calculate source offsets */
       if (pe > 0) {
          s_offsets[pe] = s_offsets[pe-1] + s_sizes[pe-1];
       }
    }
    
    /* assign source values */
    for (idx=0,pe=0; pe<shmem_n_pes(); pe++) {
       for (i=0; i<s_sizes[pe]/sizeof(long); i++) {
          source[idx++] = shmem_my_pe();
       }
    }
    
    for (i=0; i < SHMEM_ALLTOALL_SYNC_SIZE; i++) {
       pSync[i] = SHMEM_SYNC_VALUE;
    }
    
    /* wait for all PEs to initialize pSync */
    shmem_barrier_all();
    
    /* alltoallv on all PES */
    shmem_alltoallv_max(dest, max_offset, t_sizes, source,
                        s_offsets, s_sizes, 0, 0, shmem_n_pes(), pSync);
    
     /* verify results */
    for (pe=0; pe<shmem_n_pes(); pe++) {
    offset = pe * max_offset / sizeof(long);
       for (i=0; i<t_sizes[pe]/sizeof(long); i++) {
          if (dest[offset] != pe) {
             printf("[%d] ERROR: dest[%d]=%ld, should be %d\n",
                       shmem_my_pe(), offset, dest[offset], pe);
          }
             offset++;
       }
    }
    
    shmem_barrier_all();
    shfree(s_offsets);
    shfree(s_sizes);
    shfree(t_sizes);
    shfree(dest);
    shfree(source);
    
    shmem_finalize();
}
