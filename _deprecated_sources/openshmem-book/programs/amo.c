#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

// const int tries = 1000000;
int tries = 880;

#ifdef TEST64BIT
typedef long locktype;
#else
typedef int locktype;
#endif
 

int
main(int argc, char *argv[])
{
    int tpe, other;
    long i;
    struct {
	locktype a;
	locktype b;
    } *twovars;  
    long numfail = 0;
    long numfailsg = 0;
    long numfailpg = 0;

    if (argc > 1) {
      tries = atoi(argv[1]);
    }

    start_pes(0);
    tpe = 0;
    other = _num_pes() - 1;

    twovars = shmalloc(sizeof(*twovars));
    if (_my_pe() == 0) {
	printf("Element size: %ld bytes\n", sizeof(locktype));
	printf("Addresses: 1st element %p\n", &twovars->a);
	printf("           2nd element %p\n", &twovars->b);
	printf("Iterations: %d   target PE: %d   other active PE: %d\n",
		tries, tpe, other);
    }
    twovars->a = 0;
    twovars->b = 0;

    shmem_barrier_all();


    if (_my_pe() == tpe) {
	// put two values alternately to the 1st 32 bit word
	long expect, check;

	for (i=0; i<tries/2; i++) {
	    expect =  4 + i%2;
	    twovars->a = expect;
	    if (sizeof(locktype) == sizeof(int)) {
	      check = shmem_int_g((void*)&twovars->a, tpe);
	    } else if (sizeof(locktype) == sizeof(long)) {
	      check = shmem_long_g((void*)&twovars->a, tpe);
	    }
	    if (check != expect) {
		if (numfailsg < 2) {
		    printf("error: iter %ld get-after-store returned %ld expected %ld\n", i, check, expect);
		}
		numfailsg++;
		numfail++;
	    }
	}
	printf("PE %d done doing stores and gets\n",_my_pe());
	
	for (i=0; i<tries/2; i++) {
	    expect =  2 + i%2;
	    if (sizeof(locktype) == sizeof(int)) {
	      shmem_int_p((void*)&twovars->a, expect, tpe);
	      check = shmem_int_g((void*)&twovars->a, tpe);
	    } else if (sizeof(locktype) == sizeof(long)) {
	      shmem_long_p((void*)&twovars->a, expect, tpe);
	      check = shmem_long_g((void*)&twovars->a, tpe);
	    }
	    if (check != expect) {
		if (numfailpg < 2) {
		    printf("error: iter %ld get-after-put returned %ld expected %ld\n", i, check, expect);
		}
		numfailpg++;
		numfail++;
	    }
	}
	printf("PE %d done doing puts and gets\n",_my_pe());


    } else if (_my_pe() == other) {
	// keep on atomically incrementing the 2nd 32 bit word
	long oldval;

	for (i=0; i<tries; i++) {
	    if (sizeof(locktype) == sizeof(int)) {
	      oldval = shmem_int_finc((void*)&twovars->b, tpe);
	    } else if (sizeof(locktype) == sizeof(long)) {
	      oldval = shmem_long_finc((void*)&twovars->b, tpe);
	    }
	    if (oldval != i) {
		printf("error: iter %ld finc got %ld expect %ld\n", i, oldval, i);
		numfail++;
		if (numfail > 10) {
		    printf("FAIL\n");
		    abort();
		}
	    }
	}
	printf("PE %d done doing fincs\n",_my_pe());
    }
    shmem_barrier_all();
    if (numfail) {
	if (_my_pe() == tpe) {
	    printf("FAIL: %ld errors: %ld get-after-store  %ld get-after-put\n",
		 numfail, numfailsg, numfailpg);
	} else {
	    printf("FAIL: %ld errors\n", numfail);
	}
    }
    shmem_barrier_all();
    if (_my_pe() == 0) {
        printf("test complete\n");
    }
    return 0;
}
