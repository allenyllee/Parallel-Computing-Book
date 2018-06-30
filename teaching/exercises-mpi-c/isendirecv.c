/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise for Isend/Irecv
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

  double mydata=procno,leftdata=0.,rightdata=0.;
  int sendto,recvfrom;
  MPI_Request requests[4];

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  //    once to get data from the left, once from the right
  // -- for first/last process use MPI_PROC_NULL

  // get data from the left: who are you communicating with?
/**** your code here ****/
  MPI_Isend(&mydata,1,MPI_DOUBLE, sendto,0, comm,
/**** your code here ****/
	    );
  MPI_Irecv(&leftdata,1,MPI_DOUBLE, recvfrom,0, comm,
/**** your code here ****/
	    );

  // get data from the right: who are you communicating with?
/**** your code here ****/
  MPI_Isend(&mydata,1,MPI_DOUBLE, sendto,0, comm,
/**** your code here ****/
	    );
  MPI_Irecv(&rightdata,1,MPI_DOUBLE, recvfrom,0, comm,
/**** your code here ****/
	    );

  //
  // Now make sure all Isend/Irecv operations are completed
  //
/**** your code here ****/
  
  // check correctness
  mydata = mydata+leftdata+rightdata;

  double res;
  if (procno==0) {
    res = 2*procno+1;
  } else if (procno==nprocs-1) {
    res = 2*procno-1;
  } else {
    res = 3*procno;
  }

#define ISAPPROX(x,y) \
  ( ( x==0. && abs(y)<1.e-14 ) || ( y==0. && abs(x)<1.e-14 ) || \
    abs(x-y)/abs(x)<1.e-14 )
  int error=nprocs, errors;
  if (!ISAPPROX(mydata,res)) {
    printf("Data on proc %d should be %e, not %e\n",procno,res,mydata);
    error = procno;
  }
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
