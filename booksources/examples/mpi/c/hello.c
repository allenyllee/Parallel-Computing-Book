#include <stdlib.h>
#include <stdio.h>

#include "omp.h"

int main(int argc,char **argv) {

  int mythread,nthreads;
  nthreads = omp_get_num_threads();
  mythread = omp_get_thread_num();
  printf("Hello from %d out of %d outside the region\n",mythread,nthreads);

  {
    int mythread,nthreads;
    nthreads = omp_get_num_threads();
    mythread = omp_get_thread_num();
    printf("Hello from %d out of %d inside the region\n",mythread,nthreads);
  }

  {
    nthreads = omp_get_num_threads();
    mythread = omp_get_thread_num();
    printf("Hello from %d out of %d inside the nonprivate region\n",mythread,nthreads);
  }

  return 0;
}
