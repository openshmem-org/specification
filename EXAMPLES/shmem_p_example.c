#include <stdio.h>
#include <math.h>
#include <shmem.h>
static const double e = 2.71828182;
static const double epsilon = 0.00000001;

int main(void)
{
   double *f;
   int me;

   start_pes(0);
   me = _my_pe();
   f = (double *) shmalloc(sizeof (*f));

   *f = 3.1415927;
   shmem_barrier_all();

   if (me == 0)
      shmem_double_p(f, e, 1);

   shmem_barrier_all();
   if (me == 1)
      printf("%s\n", (fabs (*f - e) < epsilon) ? "OK" : "FAIL");

   return 0;
}
