#include <stdio.h>
#include <shmem.h>

int main(void)
{
  int me;

  shmem_init();  
  me = shmem_my_pe();
  printf("My PE id is: %d\n", me);

  shmem_finalize();
  return 0;
}
