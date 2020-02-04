/*
 * OpenSHMEM shmem_team_split_strided example to create a team of all even
 * ranked PEs from SHMEM_TEAM_WORLD
 */

#include <shmem.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  shmem_team_t new_team;
  shmem_team_config_t *config;

  shmem_init();
  config = NULL;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 2, npes / 2, config, 0,
                           &new_team);

  if (new_team != SHMEM_TEAM_INVALID) {
    int team_npes = shmem_team_n_pes(new_team);
    int team_mype = shmem_team_my_pe(new_team);

    if ((mype % 2 != 0) || (mype / 2 != team_mype) || (npes / 2 != team_npes)) {
      shmem_global_exit(1);
    }
  }

  shmem_finalize();
  return 0;
}
