#include <stdio.h>
#include <shmem.h>
main()
{
   start_pes(0);
   int npes = shmem_n_pes();

   if(shmem_my_pe == 0)
     printf("Number of PEs executing this application are:%d\n",npes);

   return 0;
}
