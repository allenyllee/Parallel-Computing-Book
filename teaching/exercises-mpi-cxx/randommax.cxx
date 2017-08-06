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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
using namespace std;
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
  {
    stringstream proctext;
    proctext << "Process " << setw(3) << procno << " has random value " << myrandom << endl;
    cout << proctext.str();
  }

  // Exercise part 1:
  // -- compute the maximum random value on process zero
  MPI_Reduce(
/**** your code here ****/
	     );
  if (procno==0) {
    stringstream proctext;
    proctext << "The maximum number is " << globalrandom << endl;
    cout << proctext.str();
  }

  /*
   * Exercise part 2:
   * -- compute the sum of the values, everywhere
   * -- scale your number by the sum
   * -- check that the sum of scales values is 1
   */
  float sumrandom, scaled_random;
  MPI_Allreduce(
/**** your code here ****/
		);
  scaled_random = myrandom / sumrandom;
  MPI_Allreduce(
/**** your code here ****/
		);
  if ( abs(sumrandom-1.)>1.e-14 ) {
    stringstream proctext;
    proctext << "Suspicious sum " << sumrandom << " on process " << procno << endl;
    cout << proctext.str();
  }
  if (procno==0) {
    stringstream proctext;
    proctext << "Finished" << endl;
    cout << proctext.str();
  }

  MPI_Finalize();
  return 0;
}
