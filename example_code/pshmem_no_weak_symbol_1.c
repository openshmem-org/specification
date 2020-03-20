#ifdef BUILD_PSHMEM_INTERFACES
#define SHFN(fn) p##fn
#else
#define SHFN(fn) fn
#endif
