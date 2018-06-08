#include <stddef.h>
#include <shmem.h>
#include <omp.h>

_Thread_local shmem_ctx_t thread_ctx = SHMEM_CTX_INVALID;

void lib_thread_register(void) {
  if (thread_ctx == SHMEM_CTX_INVALID)
    if (shmem_ctx_create(SHMEM_CTX_PRIVATE, &thread_ctx) &&
        shmem_ctx_create(                0, &thread_ctx))
      thread_ctx = SHMEM_CTX_DEFAULT;
}

void lib_thread_unregister(void) {
  if (thread_ctx != SHMEM_CTX_DEFAULT) {
    shmem_ctx_destroy(thread_ctx);
    thread_ctx = SHMEM_CTX_INVALID;
  }
}

void lib_thread_putmem(void *dst, const void *src, size_t nbytes, int pe) {
  shmem_ctx_putmem(thread_ctx, dst, src, nbytes, pe);
}

int main() {
  int provided;
  if (shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided))
    return 1;
  if (provided != SHMEM_THREAD_MULTIPLE)
    shmem_global_exit(2);

  const int my_pe = shmem_my_pe();
  const int n_pes = shmem_n_pes();
  const int count = 1 << 15;

  int *src_bufs[n_pes];
  int *dst_bufs[n_pes];
  for (int i = 0; i < n_pes; i++) {
    src_bufs[i] = shmem_malloc(count * sizeof(*src_bufs[i]));
    if (src_bufs[i] == NULL)
      shmem_global_exit(3);
    dst_bufs[i] = shmem_malloc(count * sizeof(*dst_bufs[i]));
    if (dst_bufs[i] == NULL)
      shmem_global_exit(4);
  }

#pragma omp parallel
  {
    int my_thrd = omp_get_thread_num();
#pragma omp for
    for (int i = 0; i < n_pes; i++)
      for (int j = 0; j < count; j++)
        src_bufs[i][j] = (my_pe << 10) + my_thrd;

    lib_thread_register();

#pragma omp for
    for (int i = 0; i < n_pes; i++)
      lib_thread_putmem(dst_bufs[my_pe], src_bufs[i],
                         count * sizeof(*src_bufs[i]), i);

    lib_thread_unregister();
  }

  shmem_finalize();
  return 0;
}
