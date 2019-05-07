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

#ifndef PARTS
#define PARTS 2
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

  // Set up data array
  double leftdata[N], myvalue[N];
  for (int i=0; i<N; i++) leftdata[i] = 0.;

  // Set up blocking for the pipeline
  int partition_starts[PARTS], partition_sizes[PARTS];
  { int points_left=N, block = N/PARTS;
    for (int ipart=0; ipart<PARTS; ipart++) {
      partition_starts[ipart] = ipart*block;
      if (ipart<PARTS-1)
	partition_sizes[ipart] = block;
      else
	partition_sizes[ipart] = points_left;
      points_left -= partition_sizes[ipart];
      if (points_left<0) {
	printf("Can not partition N=%d over PARTS=%d\n",N,PARTS);
	MPI_Abort(comm,1);
      }
    }
  }

  //
  // Exercise:
  // The code here is using blocking sends and receives.
  // Replace by non-blocking.
  //
  for (int ipart=0; ipart<PARTS; ipart++) {
/**** your code here ****/
      (
       leftdata+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,recvfrom,ipart,comm,MPI_STATUS_IGNORE);
    for (int i=partition_starts[ipart];
	 i<partition_starts[ipart]+partition_sizes[ipart];
	 i++)
      myvalue[i] = (procno+1)*(procno+1) + leftdata[i];
    MPI_Send
      (
       myvalue+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,sendto,ipart,comm);
  }

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
  double max_of_errors = 0;
  for (int i=0; i<N; i++) {
    double e = fabs( (my_sum_of_squares - myvalue[i])/myvalue[i] );
    if (e>max_of_errors) max_of_errors = e;
  }
  int
    error = max_of_errors > 1.e-12 ? procno : nprocs,
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
