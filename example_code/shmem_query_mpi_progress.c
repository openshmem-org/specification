#include <stdlib.h>
#include <shmem.h>
#include <mpi.h>

int a[1048576];

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    shmem_init();

    int mype = shmem_my_pe();

    if (!shmem_query_interoperability(SHMEM_PROGRESS_MPI))
        shmem_global_exit(EXIT_FAILURE);

    static int b = 0;
    if (mype == 0) {
        MPI_Request req = MPI_REQUEST_NULL;
        MPI_Isend(a, 1048576, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);

        while (shmem_int_atomic_fetch(&b, 0) != 1);

        MPI_Wait(&req, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(a, 1048576, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        shmem_int_atomic_set(&b, 1, 0);
    }

    shmem_finalize();
    MPI_Finalize();

    return 0;
}
