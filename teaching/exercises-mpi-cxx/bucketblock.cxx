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

#include <vector>

using namespace std;

#ifndef N
#define N 10
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

  //
  // Exercise:
  // -- do the MPI_Send and MPI_Recv calls
  //
  vector<double> leftdata(N,0.), myvalue(N,0.);
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
  int error = max_of_errors > 1.e-12 ? procno : nprocs,
    errors=-1;
  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cout << proctext.str(); proctext.clear();
  }

  MPI_Finalize();
  return 0;
}
