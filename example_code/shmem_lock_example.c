#include <stdio.h>
#include <unistd.h>
#include <shmem.h>
long L = 0;

int main(int argc, char **argv)
{
   int me, slp;
   shmem_init();
   me = shmem_my_pe();
   slp = 1;
   shmem_barrier_all();
   if (me == 1)
      sleep (3);
   shmem_set_lock(&L);
   printf("%d: sleeping %d second%s...\n", me, slp, slp == 1 ? "" : "s");
   sleep(slp);
   printf("%d: sleeping...done\n", me);
   shmem_clear_lock(&L);
   shmem_finalize();
   return 0;
}
