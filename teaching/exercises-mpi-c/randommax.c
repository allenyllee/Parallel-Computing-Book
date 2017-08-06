/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** MPI Exercise
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main() {
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Init(0,0);

  // compute communicator rank and size
/**** your code here ****/
  
  // Initialize the random number generator
  srand(procno*(double)RAND_MAX/nprocs);
  // Compute a normalized random number
  float myrandom = (rand() / (double)RAND_MAX), globalrandom;
  printf("Process %3d has random value %7.5f\n",procno,myrandom);

  // Exercise part 1:
  // -- compute the maximum random value on process zero
  MPI_Reduce(
/**** your code here ****/
	     );
  if (procno==0)
    printf("The maximum number is %7.5f\n",globalrandom);

  /*
   * Exercise part 2:
   * (re-enable this part of the code by removing the #if and #endif)
   * -- compute the sum of the values, everywhere
   * -- scale your number by the sum
   * -- check that the sum of scales values is 1
   */
#if 0
  float sumrandom, scaled_random;
  MPI_Allreduce(
/**** your code here ****/
		);
  scaled_random = myrandom / sumrandom;
  MPI_Allreduce(
/**** your code here ****/
		);
  if ( abs(sumrandom-1.)>1.e-14 )
    printf("Suspicious sum %7.5f on process %3d\n",sumrandom,procno);
#endif
  
  if (procno==0)
    printf("Success: all tests pass\n");

  MPI_Finalize();
  return 0;
}
