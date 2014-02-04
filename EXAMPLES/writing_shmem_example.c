#include <shmem.h>
#define SIZE 16
int
main(int argc, char* argv[])
{
   short  source[SIZE];
   static short  target[SIZE];
   int i;
   int num_pe = atoi(argv[1]);
   start_pes(num_pe);
   if (_my_pe(0) == 0) {
      /* initialize array */
      for(i = 0; i < SIZE; i++)
         source[i] = i;
/* local, not symmetric */
/* static makes it symmetric */
}
2
Fig. 1. RMA (Remote operation) using a Symmetric Data Object
Remote Write/Read
   /* put "size" words into target on each PE */
   for(i = 1; i < num_pe; i++)
      shmem_short_put(target, source, SIZE, i);
}
shmem_barrier_all(); /* sync sender and receiver */
if (_my_pe() != 0) {
   printf("target on PE %d is \t", _my_pe());
   for(i = 0; i < SIZE; i++)
      printf("%hd", target[i]);
   printf("\n");
}
shmem_barrier_all(); /* sync before exiting */
return 0;
