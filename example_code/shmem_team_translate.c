/*
 * OpenSHMEM shmem_team_translate example to verify the team formed by even
 * ranked PEs from SHMEM_TEAM_WORLD using the team created from
 * shmem_team_split_stride operation
 */

#include <shmem.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int                  rank, npes;
    int                  t_pe;
    int                  t_global;
    shmem_team_t         new_team;
    shmem_team_config_t *config;

    shmem_init();
    config = NULL;
    rank   = shmem_my_pe();
    npes   = shmem_num_pes();

    shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 2, npes / 2, config, 0,
                             &new_team);

    if (new_team != SHMEM_TEAM_NULL) {
        t_pe     = shmem_team_my_pe(new_team);
        t_global = shmem_team_translate_pe(new_team, t_pe, SHMEM_TEAM_WORLD);

        if (t_global != rank) {
            shmem_global_exit(1);
        }
    }

    shmem_finalize();
    return 0;
}
