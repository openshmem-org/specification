#include <stdio.h>
#include <shmem.h>
#define SIZE 16
int
main(int argc, char* argv[])
{
	short  source[SIZE];
	static short  dest[SIZE];
	int i, npes;
	shmem_init();
	npes = shmem_n_pes();
	if (shmem_my_pe() == 0) {
		/* initialize array */
		for(i = 0; i < SIZE; i++)
			source[i] = i;
		/* local, not symmetric */
		/* static makes it symmetric */
		/* put "size" words into dest on each PE */
		for(i = 1; i < npes; i++)
			shmem_short_put(dest, source, SIZE, i);
	}
	shmem_barrier_all(); /* sync sender and receiver */
	if (shmem_my_pe() != 0) {
		printf("dest on PE %d is \t", shmem_my_pe());
		for(i = 0; i < SIZE; i++)
			printf("%hd \t", dest[i]);
		printf("\n");
	}
	shmem_barrier_all(); /* sync before exiting */
	return 0;
}

