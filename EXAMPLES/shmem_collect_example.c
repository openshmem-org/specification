for (i=0; i < _SHMEM_COLLECT_SYNC_SIZE; i++) {
         pSync[i] = _SHMEM_SYNC_VALUE;
}
shmem_barrier_all(); /* Wait for all PEs to initialize pSync */
shmem_collect32(target, source, 64, pe_start, logPE_stride,
   pe_size, pSync);
