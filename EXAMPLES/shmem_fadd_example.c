int main () {
  int me, old;

  start_pes (0);
  me = _my_pe ();

  old = -1;
  dst = 22;
  shmem_barrier_all ();

  if (me == 1){
      old = shmem_int_fadd (&dst, 44, 0);
  }
  shmem_barrier_all ();
  printf ("%d: old = %d, dst = %d\n", me, old, dst);
  return 0;
}
