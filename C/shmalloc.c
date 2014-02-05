/*
* OpenSHMEM program to allocate (shmalloc) symmetric memory (1 long integer),
* and then free it.  Success of allocation is untested.
* 
* This program produces no output.
*/

#include <shmem.h>

int
main(void)
{
   long *x;
   start_pes(0);
   x = (long *) shmalloc(sizeof(*x));
   shfree(x);
   return 0;
}
