void shmem_example(/* appropriate arguments */)
{
    /* function body */
}

void pshmem_example(/* appropriate arguments */) __attribute__ ((weak, alias("shmem_example")));
