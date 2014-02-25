#include <stdio.h>
#include <shmem.h>

int
main()
{
  int npes;

  start_pes(0);
  
  npes = _num_pes();

  if (_my_pe() == 0) {
    printf("Number of PEs executing this application is: %d\n", npes);
  }

  return 0;
}
