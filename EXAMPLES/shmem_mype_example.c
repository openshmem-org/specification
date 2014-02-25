#include <stdio.h>
#include <shmem.h>

int main(void)
{
  int me;

  start_pes(0);  
  me = _my_pe();
  printf("My PE id is: %d\n", me);

  return 0;
}
