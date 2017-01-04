#include <stdio.h>
#include <unistd.h>
#include <shmem.h>

int main(void)
{
   static long lock = 0;
   int slp = 1;
   shmem_init();
   int me = shmem_my_pe();
   if (me == 1)
      sleep(3);
   shmem_set_lock(&lock);
   printf("%d: sleeping %d second(s)...\n", me, slp);
   sleep(slp);
   printf("%d: sleeping...done\n", me);
   shmem_clear_lock(&lock);
   shmem_finalize();
   return 0;
}
