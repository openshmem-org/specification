#include <stdio..h>

#include <shmem.h>

int main(int argc, char* argv[])
{
	int me, my_num_pes;
	/*
	** Starts/Initializes SHMEM/OpenSHMEM
	*/
	start_pes(0);
	/*
	** Fetch the number or processes
	** Some implementations use num_pes();
	*/
	my_num_pes = _num_pes();
	/*
	** Assign my process ID to me
	*/
	me = _my_pe();
	printf("Hello World from %d of %d\n", me, my_num_pes);
	return 0;
}
