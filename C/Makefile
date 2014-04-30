SHELL = /bin/sh

CC = oshcc
CFLAGS = -Wall -pedantic -DNDEBUG -O3

RUNCMD=oshrun
NPROCOPT=-np
NPROC=2
RUNOPT=$(NPROCOPT) $(NPROC)

C_TESTS        = circ.c cshift.c helloworld.c pi.c reduce-max.c shmem_long_finc_only.c
C_EXES         = $(C_TESTS:.c=)
EXES           = $(C_EXES)

all default:    $(EXES)

run-circ:	circ
	$(RUNCMD) $(RUNOPT) ./circ

run-cshift:	cshift
	$(RUNCMD) $(RUNOPT) ./cshift

run-helloworld:	helloworld
	$(RUNCMD) $(RUNOPT) ./helloworld

run-pi:	pi
	$(RUNCMD) $(RUNOPT) ./pi

run-reduce-max:	reduce-max
	$(RUNCMD) $(RUNOPT) ./reduce-max

run-finc:	shmem_long_finc_only
	$(RUNCMD) $(RUNOPT) ./shmem_long_finc_only

run:	all run-circ run-cshift run-helloworld run-pi run-reduce-max run-finc

clean:
	        rm -f $(EXES)
