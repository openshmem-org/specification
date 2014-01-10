#include <stdio.h>
#include <shmem.h>
main()
{
   start_pes(0);
   int me = _my_pe();

   printf("My PE id is:%d\n",me);

   return 0;
}
