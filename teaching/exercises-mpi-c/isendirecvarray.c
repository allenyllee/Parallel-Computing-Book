/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise for Isend/Irecv, sending an array
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

#define N 100
  double
    indata[N],outdata[N];
  for (int i=0; i<N; i++)
    indata[i] = 1.;

  double
    leftdata=0.,rightdata=0.;
  int sendto,recvfrom;
  MPI_Request requests[4];

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  //    once to get data from the left, once from the right
  // -- for first/last process use MPI_PROC_NULL

  // first specify left neighbour data
/**** your code here ****/
  MPI_Isend( // send data to the left
/**** your code here ****/
	    1,MPI_DOUBLE, sendto,0, comm,&(requests[0]));
  MPI_Irecv(&leftdata,1,MPI_DOUBLE, recvfrom,0, comm,&(requests[1]));

  // then the right neighbour data
/**** your code here ****/
  MPI_Isend( // send data to the right
/**** your code here ****/
	    1,MPI_DOUBLE, sendto,0, comm,&(requests[2]));
  MPI_Irecv(&rightdata,1,MPI_DOUBLE, recvfrom,0, comm,&(requests[3]));

  // make sure all irecv/isend operations are concluded
/**** your code here ****/

  /*
   * Do the averaging operation
   * Note that leftdata==rightdata==0 if not explicitly received.
   */
  for (int i=0; i<N; i++)
    if (i==0)
      outdata[i] = leftdata + indata[i] + indata[i+1];
    else if (i==N-1)
      outdata[i] = indata[i-1] + indata[i] + rightdata;
    else
      outdata[i] = indata[i-1] + indata[i] + indata[i+1];
  
  /*
   * Check correctness of the result:
   * value should be 2 at the end points, 3 everywhere else
   */
  for (int i=0; i<N; i++) {
    if ( (procno==0 && i==0) || (procno==nprocs-1 && i==N-1) ) {
      if (outdata[i]!=2)
	printf("Data on proc %d, index %d should be 2, not %d\n",
	       procno,i,outdata[i]);
    } else {
      if (outdata[i]!=3)
	printf("Data on proc %d, index %d, should be 3, not %d\n",
	       procno,i,outdata[i]);
    }
  }
  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
