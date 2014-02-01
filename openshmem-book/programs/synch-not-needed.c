...

for (i = 0; i < _SHMEM_BCAST_SYNC_SIZE; i += 1)
  {
    pSync[i] = _SHMEM_SYNC_VALUE;
  }
shmem_barrier_all ();

shmem_long_sum_to_all (target, source, npes, 0, 0, 0, npes, pSync);

...
...

shmem_barrier_all ();
