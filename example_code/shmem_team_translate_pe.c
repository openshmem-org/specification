#include <shmem.h>
#include <stddef.h>

int main(void) {
  shmem_init();
  shmem_team_config_t *config = NULL;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  shmem_team_t new_team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 2, (npes + 1) / 2, config, 0, &new_team);

  if (new_team != SHMEM_TEAM_INVALID) {
    int team_mype = shmem_team_my_pe(new_team);
    int global_mype = shmem_team_translate_pe(new_team, team_mype, SHMEM_TEAM_WORLD);

    if (global_mype != mype) {
      shmem_global_exit(1);
    }
  }

  shmem_finalize();
  return 0;
}
