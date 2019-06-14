/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise to illustrate one-sided
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

  double mydata=procno, leftdata=-2,
    window_data;

  // -- set `sendto' and `recvfrom'
  int sendto =
    ( procno<nprocs-1 ? procno+1 : MPI_PROC_NULL )
    ;

  MPI_Win the_window;
  MPI_Win_create(
/**** your code here ****/
		 MPI_INFO_NULL, comm,&the_window);

  // Exercise:
  // -- now do the Put call
  MPI_Win_fence(0,the_window);
/**** your code here ****/
  MPI_Win_fence(0,the_window);
  leftdata = window_data;

#ifdef SIMGRID
  /*
   * We can use SimGrid to do simulated timings on a single run.
   */
  MPI_Barrier(comm);
  double duration = MPI_Wtime()-starttime;
  if (procno==0)
    print("Duration with %d procs: %e\n",nprocs,duration);
#endif

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

  MPI_Win_free(&the_window);
  MPI_Finalize();

  return 0;
}
