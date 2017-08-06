/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-7
 ****
 **** MPI Exercise for Isend/Irecv
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main() {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  MPI_Init(0,0);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int mydata=1.,leftdata=0.,rightdata=0.;
  int sendto,recvfrom;
  MPI_Request requests[4];

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  //    once to get data from the left, once from the right
  // -- for first/last process use MPI_PROC_NULL

  // get data from the left: who are you communicating with?
/**** your code here ****/
  MPI_Isend(&mydata,1,MPI_INT, sendto,0, comm,
/**** your code here ****/
	    );
  MPI_Irecv(&leftdata,1,MPI_INT, recvfrom,0, comm,
/**** your code here ****/
	    );

  // get data from the right: who are you communicating with?
/**** your code here ****/
  MPI_Isend(&mydata,1,MPI_INT, sendto,0, comm,
/**** your code here ****/
	    );
  MPI_Irecv(&rightdata,1,MPI_INT, recvfrom,0, comm,
/**** your code here ****/
	    );

  //
  // Now make sure all Isend/Irecv operations are completed
  //
/**** your code here ****/
  
  // check correctness
  mydata = mydata+leftdata+rightdata;

  int error=nprocs,errors;
  if (procno==0 || procno==nprocs-1) {
    if (mydata!=2) {
      printf("Data on proc %d should be 2, not %d\n",procno,mydata);
      error = procno;
    }
  } else {
    if (mydata!=3) {
      printf("Data on proc %d should be 3, not %d\n",procno,mydata);
      error = procno;
    }
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
