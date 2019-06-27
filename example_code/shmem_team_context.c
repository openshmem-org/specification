#include <shmem.h>
#include <stdio.h>

int isum, ival;

int my_ctx_translate_pe(shmem_ctx_t src_ctx, int src_pe, shmem_ctx_t dest_ctx)
{
  if (src_ctx == SHMEM_CTX_INVALID) {
    return -1;
  }
  if (dest_ctx == SHMEM_CTX_INVALID) {
    return -1;
  }
  
  shmem_team_t src_team, dest_team;
  shmem_ctx_get_team(src_ctx, &src_team);
  shmem_ctx_get_team(dest_ctx, &dest_team);
  return shmem_team_translate(src_team, src_pe, dest_pe);
}

shmem_ctx_t my_team_create_ctx(shmem_team_t team) {
  if (team == SHMEM_TEAM_INVALID) {
    return SHMEM_CTX_INVALID;
  }

  shmem_ctx_t ctx;
  if (shmem_team_create_ctx(team, 0, &ctx) != 0) {
    fprintf (stderr, "Failed to create context for PE team\n");
    return SHMEM_CTX_INVALID;
  }
  return ctx;
}

void my_send_to_neighbor(shmem_ctx_t ctx, int *val)
{
  if (ctx == SHMEM_CTX_INVALID) {
    fprintf (stderr, "Send to neighbor fail due to invalid context\n");
    return;
  }
  
  shmem_team_t team;
  shmem_ctx_get_team(ctx, &team);
  int pe = shmem_team_my_pe(team);
  int npes = shmem_team_n_pes(team);
  int rpe = (pe + 1) % npes;

  // put my pe number in the buffer on my right hand neighbor
  shmem_ctx_int_put(ctx, val, &pe, 1, rpe);
}



int main()
{
  shmem_init();

  int npes = shmem_n_pes();
  isum = 0;
  
  shmem_team_t team_2s, team_3s;
  shmem_ctx_t ctx_2s, ctx_3s;
  shmem_team_config_t conf;
  conf.num_contexts = 1;
  long cmask = SHMEM_TEAM_NUM_CONTEXTS;

  // Create team with PEs numbered 0, 2, 4, ...
  shmem_team_spit_strided(SHMEM_TEAM_WORLD, 0, 2, npes / 2, &conf, cmask, &team_2s);
  // Sync between splits from same parent team into teams with overlapping membership
  shmem_team_sync(SHMEM_TEAM_WORLD);
  // Create team with PEs numbered 0, 3, 6, ...
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 3, npes / 3, &conf, cmask, &team_3s);

  ctx_2s = my_team_create_ctx(team_2s);
  ctx_3s = my_team_create_ctx(team_3s);

  // Send some values using the two team contexts contexts
  my_send_to_neighbor(ctx_2s, &ival2);
  my_send_to_neighbor(ctx_3s, &ival3);

  // Quiet all contexts and synchronize all PEs to complete the data transfers
  shmem_ctx_quiet(ctx_2s);  
  shmem_ctx_quiet(ctx_3s);
  shmem_team_sync(SHMEM_TEAM_WORLD);

  // We will add up some results on pe 4 of team_3s using ctx_2s
  if ((team_3s != SHMEM_TEAM_INVALID) && (team_2s != SHMEM_TEAM_INVALID)) {
    int _pe4_of_3s_in_2s = my_ctx_translate_pe(ctx_3s, 4, ctx_2s);

    if (_pe4_of_3s_in_2s < 0) {
      fprintf (stderr, "Fail to translate pe 4 from 3s context to 2s context\n");
    }
    else {
      // Add up the results on pe 4 of the 3s team, using the 2s team context
      shmem_ctx_int_atomic_add(ctx_2s, &isum, ival2 + ival3, _pe4_of_3s_in_2s);
    }
  }
  
  // Quiet the context and synchronize PEs to complete the operation
  shmem_ctx_quiet(ctx_2s);
  shmem_team_sync(SHMEM_TEAM_WORLD);

  if (shmem_team_my_pe(team_3s) == 4) {
    printf ("The total value on PE 4 of the 3s team is %d\n", isum);
  }

  // Destroy contexts before teams
  shmem_ctx_destroy(ctx_2s);
  shmem_team_destroy(team_2s);

  shmem_ctx_destroy(ctx_3s);
  shmem_team_destroy(team_3s);

  shmem_finalize();
}
