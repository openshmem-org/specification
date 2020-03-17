#include <stdint.h>
#include <shmem.h>

int main(void) {
  static uint64_t x = 0;

  shmem_init();

  int target = 0;
  shmem_ctx_t ctx;

  if (shmem_my_pe() > 0) {
    shmem_team_create_ctx(SHMEM_TEAM_WORLD, 0, &ctx);
  }
  else {
    shmem_team_create_ctx(SHMEM_TEAM_SHARED, 0, &ctx);
    target = shmem_team_translate_pe(SHMEM_TEAM_WORLD, 0, SHMEM_TEAM_SHARED);
  }

  // Undefined behavior: The following AMO may access the same
  // location concurrently using different atomicity domains.
  if (target >= 0)
    shmem_ctx_uint64_atomic_inc(ctx, &x, target);

  shmem_ctx_destroy(ctx);
  shmem_finalize();
  return 0;
}
