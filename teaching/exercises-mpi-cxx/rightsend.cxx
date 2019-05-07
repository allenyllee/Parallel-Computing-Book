/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise to illustrate sequentialization
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;
  
  MPI_Init(&argc,&argv);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

#ifdef SIMGRID
  /*
   * We can use SimGrid to do simulated timings on a single run.
   */
  MPI_Barrier(comm);
  double starttime = MPI_Wtime();
#endif

  double
    mydata=procno, // my data that will be sent right
    leftdata=-2;   // container for data that will be received

  // Exercise part 1:
  // -- set `sendto' and `recvfrom'
  // -- make sure the first and last processor are handled right!
  int sendto =
/**** your code here ****/
    ;
  int recvfrom =
/**** your code here ****/
    ;

  // Exercise 2:
  // -- now do the MPI_Send and MPI_Recv calls
  // Exercise 3:
  // -- use the MPI_Sendrecv call instead
/**** your code here ****/

#ifdef SIMGRID
  /*
   * We can use SimGrid to do simulated timings on a single run.
   */
  MPI_Barrier(comm);
  double duration = MPI_Wtime()-starttime;
  if (procno==0) {
    proctext << "Duration with " << nprocs << " procs: " << duration << endl;
    cerr << proctext.str(); proctext.clear();
  }
#endif

  /*
   * Check correctness
   */
  int
    error = procno>0 && leftdata!=mydata-1 ? procno : nprocs,
    errors = nprocs;
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cerr << proctext.str(); proctext.clear();
  }

  MPI_Finalize();
  return 0;
}
