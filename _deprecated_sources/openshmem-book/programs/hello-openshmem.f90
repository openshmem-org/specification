program hello

  include 'shmem.fh'
  integer :: shmem_my_pe, shmem_n_pes

  integer :: npes, me

  call start_pes (0)
  npes = shmem_n_pes ()
  me = shmem_my_pe ()

  write (*, 1000) me, npes

 1000 format ('Hello from', 1X, I4, 1X, 'of', 1X, I4)

end program hello
