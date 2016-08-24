CC       = oshcc
CFLAGS   = -Wall -O3

FC       = oshfort
FFLAGS   = -Wall -O3

RUNCMD   = oshrun
RUNOPT   = -np 4

C_TESTS  = $(wildcard *.c)
C_BINS   = $(C_TESTS:.c=.cx)

F_TESTS  = $(wildcard *.f90)
F_BINS   = $(F_TESTS:.f90=.fx)

.PHONY: all run clean

all: $(C_BINS) $(F_BINS)

%.cx: %.c
	$(CC) $(CFLAGS) -o $@ $+

%.fx: %.f90
	$(FC) $(FFLAGS) -o $@ $+

run: $(C_BINS)
	@for bin in $+; do				\
	    echo --$$bin------------------------------;	\
	    $(RUNCMD) $(RUNOPT) ./$$bin || exit $$?;	\
	done

clean:
	rm -f $(C_BINS) $(F_BINS)
