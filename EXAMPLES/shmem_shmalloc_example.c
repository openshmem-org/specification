#include <stdio.h>
#include <shmem.h>
main()
{
   int *target;
   int source;

   start_pes(0);

   source = shmem_my_pe();
   target = shmalloc(sizeof(int)); /*symmetric data object on heap*/

   shmem_int_put(target, source, 1, (source + 1)% shmem_n_pes());
   shmem_barrier_all();
   printf("On PE%d value of target is %d\n",source,target);

   return 0;
}
