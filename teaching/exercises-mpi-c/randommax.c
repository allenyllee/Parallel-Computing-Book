/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Init(&argc,&argv);

  // compute communicator rank and size
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);
  
  // Initialize the random number generator
  srand(procno*(double)RAND_MAX/nprocs);
  // Compute a normalized random number
  float myrandom = (rand() / (double)RAND_MAX), globalrandom;
  printf("Process %3d has random value %7.5f\n",procno,myrandom);

  /*
   * Exercise part 1:
   * -- compute the sum of the values, everywhere
   * -- scale your number by the sum
   * -- check that the sum of the scaled values is 1
   */
  float sumrandom, scaled_random, sum_scaled_random;
  MPI_Allreduce(
/**** your code here ****/
		);
  scaled_random = myrandom / sumrandom;
  MPI_Allreduce(
/**** your code here ****/
		);

  /*
   * Correctness test
   */
  int error=nprocs, errors;
  if ( abs(sum_scaled_random-1.)>1.e-5 ) {
    printf("Suspicious sum %7.5f on process %3d\n",sum_scaled_random,procno);
    error = procno;
  }
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      printf("Part 1 finished; all results correct\n");
    else
      printf("Part 1: first error occurred on rank %d\n",errors);
  }

#if 0
  // Exercise part 2:
  // -- compute the maximum random value on process zero
  MPI_Reduce(
/**** your code here ****/
	     );
  if (procno==0)
    printf("Part 2: The maximum number is %7.5f\n",globalrandom);
#endif

  MPI_Finalize();
  return 0;
}
