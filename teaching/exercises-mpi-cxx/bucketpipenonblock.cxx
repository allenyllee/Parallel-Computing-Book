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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <mpi.h>

// headers just for this exercise
#include <algorithm>
#include <vector>

using namespace std;

#ifndef N
#define N 10
#endif

#ifndef PARTS
#define PARTS 2
#endif

int main(int argc,char **argv) {

  stringstream proctext;

  MPI_Init(&argc,&argv);

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
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
  vector<double> leftdata(N,0.), myvalue(N,0.);

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
	proctext << "Can not partition N=" << N << " over PARTS=" << PARTS << endl;
	cerr << proctext.str(); proctext.clear();
	MPI_Abort(comm,1);
      }
    }
  }

  //
  // Exercise:
  // The code here is using blocking sends and receives.
  // Replace by non-blocking.
  //
#if 0
  for (int ipart=0; ipart<PARTS; ipart++) {
    MPI_Recv
      (
       leftdata.data()+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,recvfrom,ipart,comm,MPI_STATUS_IGNORE);
    for (int i=partition_starts[ipart];
	 i<partition_starts[ipart]+partition_sizes[ipart];
	 i++)
      myvalue[i] = (procno+1)*(procno+1) + leftdata[i];
    MPI_Send
      (
       myvalue.data()+partition_starts[ipart],partition_sizes[ipart],
       MPI_DOUBLE,sendto,ipart,comm);
  }
#endif
/**** your code here ****/

#ifdef SIMGRID
  /*
   * We can use SimGrid to do simulated timings on a single run.
   */
  MPI_Barrier(comm);
  double duration = MPI_Wtime()-starttime;
  if (procno==0) {
    proctext << "Duration with " << nprocs << " procs: " << duration << endl;
    cout << proctext.str(); proctext.clear();
  }
#endif

  /*
   * Check correctness
   */
  double p1 = procno+1.;
  double my_sum_of_squares = p1*p1*p1/3 + p1*p1/2 + p1/6;
  double max_of_errors =
    *max_element(myvalue.begin(),myvalue.end(),
		 [my_sum_of_squares] (double x,double y) -> bool {
		   return fabs( (my_sum_of_squares-x)/x ) < fabs( (my_sum_of_squares-y)/y ); }
		 );
  int
    error = max_of_errors > 1.e-12 ? procno : nprocs,
    errors=-1;
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
