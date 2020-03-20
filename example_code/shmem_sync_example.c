#include <shmem.h>
#include <stdio.h>

int main(void) {
  static int x = 10101;

  shmem_team_t twos_team = SHMEM_TEAM_INVALID;
  shmem_team_t threes_team = SHMEM_TEAM_INVALID;
  shmem_team_config_t *config = NULL;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes > 2)
    shmem_team_split_strided(SHMEM_TEAM_WORLD, 2, 2, (npes - 1) / 2, config, 0, &twos_team);

  if (npes > 3)
    shmem_team_split_strided(SHMEM_TEAM_WORLD, 3, 3, (npes - 1) / 3, config, 0,
                             &threes_team);

  int mype_twos = shmem_team_my_pe(twos_team);
  int mype_threes = shmem_team_my_pe(threes_team);
  int npes_twos = shmem_team_n_pes(twos_team);
  int npes_threes = shmem_team_n_pes(threes_team);

  if (twos_team != SHMEM_TEAM_INVALID) {
    /* put the value 2 to the next team member in a circular fashion */
    shmem_p(
        &x, 2,
        shmem_team_translate_pe(twos_team, (mype_twos + 1) % npes_twos, SHMEM_TEAM_WORLD));
    shmem_quiet();
    shmem_sync(twos_team);
  }

  shmem_sync(SHMEM_TEAM_WORLD);

  if (threes_team != SHMEM_TEAM_INVALID) {
    /* put the value 3 to the next team member in a circular fashion */
    shmem_p(&x, 3,
            shmem_team_translate_pe(threes_team, (mype_threes + 1) % npes_threes,
                                    SHMEM_TEAM_WORLD));
    shmem_quiet();
    shmem_sync(threes_team);
  }

  if (mype && mype % 3 == 0) {
    if (x != 3)
      shmem_global_exit(3);
  }
  else if (mype && mype % 2 == 0) {
    if (x != 2)
      shmem_global_exit(2);
  }
  else if (x != 10101) {
    shmem_global_exit(1);
  }

  shmem_finalize();
  return 0;
}
