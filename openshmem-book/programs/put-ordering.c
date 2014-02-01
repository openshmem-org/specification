if ( _my_pe () == 0 )
  {
    shmem_int_put (&d1, &s1, 64, 1);   /* 64 ints to PE 1 */
    shmem_int_put (&d2, &s2, 8, 2);    /* different 8 ints to PE 2 */
  }
shmem_barrier_all ();
