/* CVS info   */
/* $RCSfile: all2all_main.c,v $  */
/*  
* Purpose: all2all.c copies data from one half of a table to the other 
*	   half of the table. 
*
*
*   Date      Description
*
*             all2all has been modified to automatically compare cksum results
*             for 128 processors at run time and to print an error message 
*	      if there is a discrepancy.
*	      In the future additional error checking for any number of processors
*	      will be done. 
*             added memset(tab,0,tsize)
*
* Preprocessor DEFINED Variables:
*	1. This benchmark will automatically verify checksums unless CHECKOFF is
*	defined in the makefile flags. To turn off the automated check specify
*	-DCHECKOFF in the makefile flags and recompile.
*	(MUST define CHECKOFF if not using 128 processors.)
*	2. If additional timing info is needed for debugging specify -DPTIMES in
*	the makfile flags.
*/

#include <string.h>
#include "all2all.h"

int64 SELF, SIZE;

#if 0
int64 known_v[] = {
 0x889d1f6f6b165117,
 0xc2597eee7a77503b,
 0x9fde67a85fec3140,
 0x98218560b0e2fcad,
 0x77970e91ec2ae92f,
 0xd7c257a76e652480,
 0xfae8fc3473e44bd7,
 0xae70524b190b97d1,
 0xbd3481e6d55c2587,
 0x92b1e34c9a63c162,
 0xd53483207d373375,
 0x818b5ae39e15de0c,
 0xa10c2c69b3441650,
 0x3213b203ef570cfe,
 0x953cacafbc6694af,
 0x0435c6359cfeac6a,
 0x0107162b374ac090,
 0x3b4579d543eb131e,
 0x1f46dbcd8e23ca22,
 0x4f99bd5b1c45bff2,
 0x69872eca2dd09002,
 0x5a10168c91da8c2e,
 0xfb7842751192f1bf,
 0x42d182c4447097fe,
 0xacdb47e7a6c94a44,
 0x91fb985dbdd6e93b,
 0x4796404dd92f2c3a,
 0xcda282a270d3610f,
 0x29d786ca8abdaf09,
 0x3f9af62d5a02bdc6,
 0x513eb2b11ab80a05,
 0x59a32e0cc53f2c3d,
 0x5b22688cc292ee8c,
 0xd7076df7f4c3b35b,
 0x3dcf8e920a889b72,
 0x6cf0fe53b376b881
};

#endif
int gv = 0;

/* Set up for one iteration only.*/
int64 ckv[3] =  {
   0x156a0e1af0914226,
   0xa70ebc57a39fd98d,
   0x1513f274d76734c6,
};

uint64 do_cksum        (uint64 *arr, int64 len)
{
   int64 i, cksum;
  
   // compute src cksum
   for (i = cksum = 0; i < len; i++)
      cksum += arr[i];
   return accum_long (cksum);
}

int main (int argc, char *argv[])
{
   static char cvs_info[] = "BMkGRP $Date: $ $Revision: $ $RCSfile: all2all_main.c,v $ $Name: $";

   int itr;
   int idx;
   brand_t br;
   timer t, t0, t1;
   double nsec;

   double total_time = 0.0;
  
   int status = 0;
  
   int64 i, seed, arg, msize, tsize, len, oldsize=0, rep, cksum;
   uint64 *tab=NULL;
  
   start_pes(0);
   SELF=_my_pe();
   SIZE=_n_pes();

   if (argc < 5) {
      if (SELF == 0)
         fprintf (stderr, "Usage:\t%s seed msg_size(B) table_size(MB) rep_cnt "
	          "[ms2 ts2 rc2 ..]\n", argv[0]);
         status = 1;
      goto DONE;
   }
   seed = atol (argv[1]);
   if (SELF == 0)
      printf ("base seed is %ld\n", seed);
   seed += SELF << 32;
   brand_init (&br, seed);  // seed uniquely per PE

   arg = 2;
  
   while (arg < argc) {
    
    
      msize = atol (argv[arg++]);               if (arg >= argc)  break;
      /* Table size * 1 million. */
      tsize = atol (argv[arg++]) * (1L << 20);  if (arg >= argc)  break;
      //rep   = atol (argv[arg++]);
      rep   = 1;
      arg++;

      if (SELF == 0)  printf ("tsize = %ldMB  msize = %dB\n",
			        tsize/(1L<<20), msize);
      if (msize < sizeof(long)) {
         if (SELF == 0)  printf ("msize too short!\n");
         //status = 1;
         goto DONE;
      }
      //itr=0;

      idx = 0;

      switch(SIZE){
         case 2:
            idx = 0;
            break;
         case 4:
            idx = 1;
            break;
         case 8:
            idx = 2;
            break;
         default:
            fprintf(stderr,"warning, check sum for (%d) pes not supported.\n",
                        SIZE);
      }
 
      while (rep-- > 0) {

         /* START TIMING */
         //timer_clear (&t0);
         //timer_clear (&t1);
         //timer_start (&t0);
	    
         if ((tab == NULL) || (tsize > oldsize)) {
	   if (tab != NULL) {
	      dram_shfree (tab);
	      oldsize = 0;
	   }
	   if (SELF == 0)  printf ("trying dram_shmalloc of %ld bytes\n", tsize);
	   tab = (uint64 *) dram_shmalloc (tsize);
	
	   if (tab == NULL) {
	      if (SELF == 0)  printf ("dram_shmalloc failed!\n");
               status = 1;
	      goto DONE;
	  
	   }
	   oldsize = tsize;
         }
      
         // length in words
         len = tsize / sizeof(uint64);
      
         // important to init table
         // to ensure cksum consistency on different platforms
         memset(tab,0,tsize);
      
         for (i = 0; i < len; i+=64){
            tab[i] = brand(&br);
         }
     
         // we'll have destination/source arrays each of half size
         len /= 2;

         //timer_stop (&t0);
         // source checksum
         cksum = do_cksum (&tab[len], len);
         if (SELF == 0)  printf ("cksum is %016lx\n", cksum);
         if (SELF == 0){
            //if(cksum!=ckv[itr++]){
            /* Set up for one iteration only. */
            if(cksum!=ckv[idx]){
               printf ("cksum  %016lx != ckv[%d] %016x\n",cksum,idx,ckv[idx]);
	         gexit(1);
	   }
          
         }      	      

         //timer_start (&t1);
         len = do_all2all (&tab[0], &tab[len], len, msize/sizeof(uint64));
      
         shmem_barrier_all();
      
         //timer_stop (&t1);
         /* END TIMING */
#if 0

         // dest checksum
         i = do_cksum (&tab[0], len);
         if (i != cksum) {
	   printf ("PE %4ld  ERROR: %016lx != %016lx\n", SIZE, i, cksum);
	   status = 1;
	   goto DONE;
         }

#ifndef CHECKOFF
         if (i != known_v[gv]) {
            printf ("CHECKSUM PE %4ld  ERROR: %016lx != %016lx\n", SIZE, i, known_v[gv]);
            status = 1;
            goto DONE;
         }
         gv++;
#endif


         //t.accum_wall = t0.accum_wall + t1.accum_wall;
         //t.accum_cpus = t0.accum_cpus + t1.accum_cpus;


         /*if (SELF == 0) {

#ifdef PTIMES
	   printf ("%8.3f %8.3f\n",    t0.accum_wall , t1.accum_wall);
	   printf ("%8.3f %8.3f\n",    t0.accum_cpus , t1.accum_cpus);
#endif
	   printf ("wall reports %8.3f secs  cpus report %8.3f secs\n",
		    t.accum_wall, t.accum_cpus);
	   nsec = MAX(t.accum_wall, t.accum_cpus);
            total_time += nsec;
	   if (nsec > 0)
	     printf ("%8.3f MB/sec with %ld bytes transfers\n",
		     len*sizeof(uint64)/(double)(1L<<20)/nsec, msize);
         }*/
#endif
      }
      //if (SELF == 0)
         //printf ("\n");
   }
   //if (SELF == 0)
   //{
      //printf ("total time = %14.9f\n", total_time);

   //}
 
 DONE: 
   shmem_barrier_all();
   return status;
}


