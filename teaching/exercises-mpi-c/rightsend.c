/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise to illustrate sequentialization
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
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
  if (procno==0)
    printf("Duration with %d procs: %e\n",nprocs,duration);
#endif

  /*
   * Check correctness
   */
  int
    error = procno>0 && leftdata!=mydata-1 ? procno : nprocs,
    errors=-1;
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      printf("Finished; all results correct\n");
    else
      printf("First error occurred on proc %d\n",errors);
  }

  MPI_Finalize();
  return 0;
}
