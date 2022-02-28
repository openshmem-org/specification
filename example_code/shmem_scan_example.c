#include <shmem.h>

int collect_at(shmem_team_t team, void *dest, const void *source, size_t nbytes, int who) {
  static size_t sym_nbytes;
  sym_nbytes = nbytes;
  shmem_team_sync(team);
  int rc = shmem_sum_exscan(team, &sym_nbytes, &sym_nbytes, 1);
  shmem_putmem((void *)((uintptr_t)dest + sym_nbytes), source, nbytes, who);
  shmem_quiet();
  shmem_team_sync(team);
  return rc;
}
