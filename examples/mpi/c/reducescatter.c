/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%% reducescatter.c : simple illustration of reduce_scatter
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  // record what processes you will communicate with
  int *recv_requests;
  int *counts,nrecv_requests=0,nsend_requests;
  recv_requests = (int*) malloc(ntids*sizeof(int));
  counts = (int*) malloc(ntids*sizeof(int));
  for (int i=0; i<ntids; i++) {
    recv_requests[i] = 0;
    counts[i] = 1;
  }

  // generate random requests
  for (int i=0; i<ntids; i++)
    if ( (float) rand()/(float)RAND_MAX < 2./ntids ) {
      recv_requests[i] = 1; nrecv_requests++;
    }
  printf("[%d]: ",mytid);
  for (int i=0; i<ntids; i++)
    printf("%d ",recv_requests[i]);
  printf("\n");
  // find how many procs want to communicate with you
  MPI_Reduce_scatter
    (recv_requests,&nsend_requests,counts,MPI_INT,
    MPI_SUM,comm);
  printf("[%d]: %d requests\n",mytid,nsend_requests);
  MPI_Request *mpi_requests,irequest=0;
  mpi_requests = (MPI_Request*) malloc((nrecv_requests+nsend_requests)*sizeof(MPI_Request));
  int msg=1;
  // send a msg to the selected processes
  for (int i=0; i<ntids; i++)
    if (recv_requests[i]>0)
      MPI_Isend(&msg,1,MPI_INT, /*to:*/ i,0,comm,
        mpi_requests+irequest++);
  // do as many receives as you know are coming in
  for (int i=0; i<nsend_requests; i++)
    MPI_Irecv(&msg,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,comm,
        mpi_requests+irequest++);
  MPI_Waitall(irequest,mpi_requests,MPI_STATUSES_IGNORE);

  MPI_Finalize();
  return 0;
}
