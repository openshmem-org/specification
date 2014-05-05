SHELL = /bin/sh

CC = oshcc
CFLAGS = -Wall -O3

RUNCMD=oshrun
NPROCOPT=-np

C_TESTS        = $(wildcard *.c)
C_EXES         = $(C_TESTS:.c=)
EXES           = $(C_EXES)

all default:    $(EXES)

run-hello:	hello-openshmem
	$(RUNCMD) $(NPROCOPT) 2 ./hello-openshmem

run-cswap:	shmem_cswap_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_cswap_example

run-fadd:	shmem_fadd_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_fadd_example

run-finc:	shmem_finc_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_finc_example

run-g:	shmem_g_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_g_example

run-inc:	shmem_inc_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_inc_example

run-iput:	shmem_iput_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_iput_example

run-lock:	shmem_lock_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_lock_example

run-mype:	shmem_mype_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_mype_example

run-npes:	shmem_npes_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_npes_example

run-p:	shmem_p_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_p_example

run-ptr:	shmem_ptr_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_ptr_example

run-put:	shmem_put_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_put_example

run-shmalloc:	shmem_shmalloc_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_shmalloc_example

run-swap:	shmem_swap_example
	$(RUNCMD) $(NPROCOPT) 2 ./shmem_swap_example

run-writing:	writing_shmem_example
	$(RUNCMD) $(NPROCOPT) 2 ./writing_shmem_example

run-barrierall:	shmem_barrierall_example
	$(RUNCMD) $(NPROCOPT) 4 ./shmem_barrierall_example

run-barrier:	shmem_barrier_example
	$(RUNCMD) $(NPROCOPT) 4 ./shmem_barrier_example

run-broadcast:	shmem_broadcast_example
	$(RUNCMD) $(NPROCOPT) 4 ./shmem_broadcast_example

run-collect:	shmem_collect_example
	$(RUNCMD) $(NPROCOPT) 4 ./shmem_collect_example

run-fence:	shmem_fence_example
	$(RUNCMD) $(NPROCOPT) 4 ./shmem_fence_example

run-quiet:	shmem_quiet_example
	$(RUNCMD) $(NPROCOPT) 4 ./shmem_quiet_example

run:	all run-hello run-barrierall run-barrier run-broadcast run-collect run-cswap run-fadd run-fence run-finc run-g run-inc run-iput run-lock run-mype run-npes run-p run-ptr run-put run-quiet run-shmalloc run-swap run-writing

clean:
	        rm -f $(EXES)
