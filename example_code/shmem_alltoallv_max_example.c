#include <shmem.h>
#include <stdio.h>

main()
{
    static long pSync[SHMEM_ALLTOALL_SYNC_SIZE];
    long *source, *dest;
    size_t *s_offsets, *s_nelems, max_offset, *t_nelems, offset;
    int  i, maxcount, pe, idx;
    
    start_pes(0);
    
    maxcount = shmem_n_pes();
    max_offset = maxcount;
    dest = (long *) shmalloc(maxcount * shmem_n_pes() * sizeof(long));
    source = (long *) shmalloc(maxcount * shmem_n_pes() * sizeof(long));
    s_offsets  = (size_t *) shmalloc(shmem_n_pes() * sizeof(size_t));
    s_nelems   = (size_t *) shmalloc(shmem_n_pes() * sizeof(size_t));
    t_nelems   = (size_t *) shmalloc(shmem_n_pes() * sizeof(size_t));
    
    s_offsets[0] = 0;
    for (pe=0; pe <shmem_n_pes(); pe++) {
    
       /* set source sizes, no need to set dest sizes */
       s_nelems[pe] = pe;
       t_nelems[pe] = 0;
    
       /* calculate source offsets */
       if (pe > 0) {
          s_offsets[pe] = s_offsets[pe-1] + s_nelems[pe-1];
       }
    }
    
    /* assign source values */
    for (idx=0,pe=0; pe<shmem_n_pes(); pe++) {
       for (i=0; i<s_nelems[pe]; i++) {
          source[idx++] = shmem_my_pe();
       }
    }
    
    for (i=0; i < SHMEM_ALLTOALL_SYNC_SIZE; i++) {
       pSync[i] = SHMEM_SYNC_VALUE;
    }
    
    /* wait for all PEs to initialize pSync */
    shmem_barrier_all();
    
    /* alltoallv on all PES */
    shmem_alltoallv_max64(dest, max_offset, t_nelems, source,
                        s_offsets, s_nelems, 0, 0, shmem_n_pes(), pSync);
    
     /* verify results */
    for (pe=0; pe<shmem_n_pes(); pe++) {
    offset = pe * max_offset;
       for (i=0; i<t_nelems[pe]; i++) {
          if (dest[offset] != pe) {
             printf("[%d] ERROR: dest[%d]=%ld, should be %d\n",
                       shmem_my_pe(), offset, dest[offset], pe);
          }
             offset++;
       }
    }
    
    shmem_barrier_all();
    shfree(s_offsets);
    shfree(s_nelems);
    shfree(t_nelems);
    shfree(dest);
    shfree(source);
    
    shmem_finalize();
}
