/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise for sendrecv : three-point combination
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

  double mydata=procno, leftdata=0, rightdata=0;
  int sendto,recvfrom;

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  // -- once to get data from the left, once from the right

  // first get left neighbour data
  //hint:  sendto = 
  //hint:  recvfrom =
/**** your code here ****/
  MPI_Sendrecv
    (&mydata,1,MPI_DOUBLE, sendto,0,
     &leftdata,1,MPI_DOUBLE, recvfrom,0, comm,MPI_STATUS_IGNORE);

  // then the right neighbour data
  //hint:  recvfrom =
  //hint:  sendto = 
/**** your code here ****/
  MPI_Sendrecv
    (&mydata,1,MPI_DOUBLE, sendto,0,
     &rightdata,1,MPI_DOUBLE, recvfrom,0, comm,MPI_STATUS_IGNORE);

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
