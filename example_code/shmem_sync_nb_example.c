#include <shmem.h>

int main(void) {
  static int x = 10101;

  shmem_team_t twos_team = SHMEM_TEAM_INVALID;
  shmem_team_config_t *config = NULL;
  shmem_req_h req_twos = SHMEM_REQ_INVALID;
  shmem_req_h req_world = SHMEM_REQ_INVALID;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes > 2)
    shmem_team_split_strided(SHMEM_TEAM_WORLD, 2, 2, (npes - 1) / 2, config, 0, &twos_team);

  if (twos_team != SHMEM_TEAM_INVALID) {
    int mype_twos = shmem_team_my_pe(twos_team);
    int npes_twos = shmem_team_n_pes(twos_team);
    shmem_p(&x, 2,
            shmem_team_translate_pe(twos_team, (mype_twos + 1) % npes_twos, SHMEM_TEAM_WORLD));
    shmem_quiet();
  }

  /* Overlap: initiate world sync while twos_team sync proceeds */
  if (twos_team != SHMEM_TEAM_INVALID) {
    shmem_sync_nb(twos_team, &req_twos);
  }
  shmem_sync_nb(SHMEM_TEAM_WORLD, &req_world);
  if (req_twos != SHMEM_REQ_INVALID) {
    shmem_req_wait(&req_twos);
  }
  shmem_req_wait(&req_world);

  if (mype && mype % 2 == 0) {
    if (x != 2) {
      shmem_global_exit(2);
    }
  } else if (x != 10101) {
    shmem_global_exit(1);
  }

  shmem_finalize();
  return 0;
}
