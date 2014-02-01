/*
**  Required includes and definitions
*/

#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

/*
**  LP64 types
*/

typedef   signed  char  int8 ;
typedef unsigned  char  uint8 ;
typedef          short  int16;
typedef unsigned short  uint16;
typedef            int  int32;
typedef unsigned   int  uint32;
typedef           long  int64;
typedef unsigned  long  uint64;

/*
**  timing
*/

#include <time.h>

typedef struct {
  double accum_wall, accum_cpus;
  double start_wall, start_cpus;
  time_t init_time;
  char running;
} timer;

void timer_clear     (timer *t);
void timer_start     (timer *t);
void timer_stop      (timer *t);
void timer_report    (timer *t, double *pwall, double *pcpus, int64 print);

/* 
**  some masking macros
*/

#define _ZERO64       0uL
#define _maskl(x)     (((x) == 0) ? _ZERO64   : ((~_ZERO64) << (64-(x))))
#define _maskr(x)     (((x) == 0) ? _ZERO64   : ((~_ZERO64) >> (64-(x))))
#define _mask(x)      (((x) < 64) ? _maskl(x) : _maskr(2*64 - (x)))

/* 
**  PRNG
*/

#define _BR_RUNUP_      128L
#define _BR_LG_TABSZ_    7L
#define _BR_TABSZ_      (1L<<_BR_LG_TABSZ_)

typedef struct {
  uint64 hi, lo, ind;
  uint64 tab[_BR_TABSZ_];
} brand_t;

#define _BR_64STEP_(H,L,A,B) {\
  uint64 x;\
  x = H ^ (H << A) ^ (L >> (64-A));\
  H = L | (x >> (B-64));\
  L = x << (128 - B);\
}

uint64 brand         (brand_t *p);
void   brand_init    (brand_t *p, uint64 val);

/* 
**  shmem functionality
*/

/* 
**  max size of tab (in wrds)
*/

#ifndef LG_NWRDS
#define LG_NWRDS  24L
#endif
#define NWRDS    (1uL << LG_NWRDS)

#include <shmem.h>

#define MY_GTHREAD shmem_my_pe()
#define GTHREADS   shmem_n_pes()
#define TYPE       uint64

TYPE *mpp_alloc      (int64 nbytes); 
void  mpp_free       (TYPE *ptr);
int64 mpp_accum_long (int64 val);
void  do_sync_init   (void);
int64 all2all        (TYPE *dst, uint64 *src, int64 len, int64 nwrd);
void  do_all2all     (TYPE *tab, uint64 *loc, brand_t *br, int64 msize,
		      int64 tsize, int64 rep, int64 print);
void  do_warmup      (brand_t *br);


/*
**  driver program
*/

int main (int argc, char *argv[])

{
  brand_t br;
  int64 seed, arg, msize, tsize, rep;
  TYPE *tab;
  uint64 *loc;

  start_pes(0);

  if (argc < 5) {
    if (MY_GTHREAD == 0)
      fprintf (stderr, "Usage:\t%s seed msg_size(B) table_size(MB) rep_cnt "
        "[ms2 ts2 rc2 ..]\n", argv[0]);
      goto DONE;
  }

/* 
**  alloc some shared space
**  (checks for valid pointer and casts)
*/

  tab = mpp_alloc (NWRDS * sizeof(uint64));

/* 
**  pointer to local space
*/

  loc = &tab[0];


/* 
**  init all local memory
*/

  bzero ((void *)&loc[0], NWRDS * sizeof(uint64));

  seed = atol (argv[1]);
  if (MY_GTHREAD == 0)
    printf ("base seed is %ld\n", seed);
  seed += (uint64)MY_GTHREAD << 32;

/*
**  seed uniquely per PE
*/

  brand_init (&br, seed);

  arg = 2;

  while (arg < argc) {
    msize = atol (argv[arg++]);
    if (arg >= argc)
      break;
    tsize = atol (argv[arg++]) * (1L << 20);
    if (arg >= argc)
      break;
    rep   = atol (argv[arg++]);

    if (MY_GTHREAD == 0)
      printf ("tsize = %ldMB  msize = %5ldB\n", tsize/(1L<<20), msize);
    if (msize < sizeof(long)) {
      if (MY_GTHREAD == 0)
	printf ("msize must be > %ld B\n", (int64)sizeof(long));
      goto DONE;
    }
    if (tsize > (NWRDS * sizeof(long))) {
      if (MY_GTHREAD == 0)
	printf ("tsize must be < %ld MiB\n",
		(int64)(NWRDS * sizeof(long)) / (1uL<<20));
      goto DONE;
    }

/* 
**  exits on error
*/

    do_all2all (tab, loc, &br, msize, tsize, rep, 1);
    
    if (MY_GTHREAD == 0)
      printf ("\n");
  }

/* 
**  free up the shared memory
*/

  mpp_free (tab);

 DONE:
  shmem_barrier_all();
  return 0;
}
