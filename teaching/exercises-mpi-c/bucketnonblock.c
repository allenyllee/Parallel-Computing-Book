/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise to illustrate pipelining
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#ifndef N
#define N 10
#endif

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

  // Set `sendto' and `recvfrom'
  int sendto =
    ( procno<nprocs-1 ? procno+1 : MPI_PROC_NULL )
    ;
  int recvfrom =
    ( procno>0 ? procno-1 : MPI_PROC_NULL )
    ;

  //
  // Exercise:
  // -- do the MPI_Send and MPI_Recv calls
  //
  double leftdata[N], myvalue[N];
  for (int i=0; i<N; i++) leftdata[i] = 0.;
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
  double p1 = procno+1.;
  double my_sum_of_squares = p1*p1*p1/3 + p1*p1/2 + p1/6;
  int
    error = fabs( (my_sum_of_squares - myvalue[N-1])/myvalue[N-1] ) > 1.e-12 ? procno : nprocs,
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
