void pshmem_example(/* appropriate arguments */)
{
    /* function body */
}

void shmem_example(/* appropriate arguments */) __attribute__ ((weak, alias("pshmem_example")));
