SHELL = /bin/sh

CC = oshcc
CFLAGS = -Wall -O3

RUNCMD=oshrun
NPROCOPT=-np

C_TESTS        = $(wildcard *.c)
C_EXES         = $(C_TESTS:.c=)
EXES           = $(C_EXES)

all default:    $(EXES)

run-helloworld run-cswap run-fadd run-finc run-g run-inc run-iput run-lock run-mype run-npes run-p run-ptr run-put run-shmalloc run-swap run-writing:
	@$(CC) $(CFLAGS) -o shmem_`echo $@ | awk '{sub(/^run-/, ""); print}'`_example shmem_`echo $@ | awk '{sub(/^run-/, ""); print}'`_example.c
	@$(RUNCMD) $(NPROCOPT) 2 ./shmem_`echo $@ | awk '{sub(/^run-/, ""); print}'`_example

run-barrierall run-barrier run-broadcast run-collect run-fence run-quiet:
	@$(CC) $(CFLAGS) -o shmem_`echo $@ | awk '{sub(/^run-/, ""); print}'`_example shmem_`echo $@ | awk '{sub(/^run-/, ""); print}'`_example.c
	@$(RUNCMD) $(NPROCOPT) 4 ./shmem_`echo $@ | awk '{sub(/^run-/, ""); print}'`_example

run:	all run-helloworld run-barrierall run-barrier run-broadcast run-collect run-cswap run-fadd run-fence run-finc run-g run-inc run-iput run-lock run-mype run-npes run-p run-ptr run-put run-quiet run-shmalloc run-swap run-writing

clean:
	        rm -f $(EXES)
