/* circular shift bbb into aaa */
#include <shmem.h>
int aaa, bbb;
int main (int argc, char * argv[])
{
    start_pes(0);
    shmem_int_get(&aaa, &bbb, 1,(_my_pe() + 1)% _num_pes());
    shmem_barrier_all();
    return 0;
}

