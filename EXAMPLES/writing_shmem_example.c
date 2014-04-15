#include <stdio.h>
#include <shmem.h>
#define SIZE 16
int
main(int argc, char* argv[])
{
	short  source[SIZE];
	static short  target[SIZE];
	int i, npes;
	start_pes(0);
	npes = _num_pes();
	if (_my_pe() == 0) {
		/* initialize array */
		for(i = 0; i < SIZE; i++)
			source[i] = i;
		/* local, not symmetric */
		/* static makes it symmetric */
		/* put "size" words into target on each PE */
		for(i = 1; i < npes; i++)
			shmem_short_put(target, source, SIZE, i);
	}
	shmem_barrier_all(); /* sync sender and receiver */
	if (_my_pe() != 0) {
		printf("target on PE %d is \t", _my_pe());
		for(i = 0; i < SIZE; i++)
			printf("%hd \t", target[i]);
		printf("\n");
	}
	shmem_barrier_all(); /* sync before exiting */
	return 0;
}

