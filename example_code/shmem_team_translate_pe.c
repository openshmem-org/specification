#include <shmem.h>
#include <stddef.h>

int main(void) {
  int my_pe;
  int npes;
  int t_pe;
  int t_global;
  shmem_team_t new_team;
  shmem_team_config_t *config;

  shmem_init();
  config = NULL;
  my_pe = shmem_my_pe();
  npes = shmem_n_pes();

  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 2, (npes + 1) / 2, config, 0,
                           &new_team);

  if (new_team != SHMEM_TEAM_INVALID) {
    t_pe = shmem_team_my_pe(new_team);
    t_global = shmem_team_translate_pe(new_team, t_pe, SHMEM_TEAM_WORLD);

    if (t_global != my_pe) {
      shmem_global_exit(1);
    }
  }

  shmem_finalize();
  return 0;
}
