/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% cancel.c : cancelling a send operation
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int procno,nprocs, first_tid,ierr,dummy[11];
  MPI_Comm comm;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno);
  MPI_Comm_size(comm,&nprocs);

  // Initialize the random number generator
  srand((int)(procno*(double)RAND_MAX/nprocs));

  fprintf(stderr,"get set, go!\n");
  if (procno==nprocs-1) {
    MPI_Status status;
    MPI_Recv(dummy,0,MPI_INT, MPI_ANY_SOURCE,0,comm,
                    &status); 
    first_tid = status.MPI_SOURCE;
    MPI_Bcast(&first_tid,1,MPI_INT, nprocs-1,comm); 
    fprintf(stderr,"[%d] first msg came from %d\n",procno,first_tid);
  } else {
    float randomfraction = (rand() / (double)RAND_MAX);
    int randomwait = (int) ( nprocs * randomfraction );
    MPI_Request request;
    fprintf(stderr,"[%d] waits for %e/%d=%d\n",
	   procno,randomfraction,nprocs,randomwait);
    sleep(randomwait);
    MPI_Isend(dummy,0,MPI_INT, nprocs-1,0,comm,
                     &request); 
    MPI_Bcast(&first_tid,1,MPI_INT, nprocs-1,comm
                    ); 
    if (procno!=first_tid) {
      MPI_Cancel(&request); 
      fprintf(stderr,"[%d] canceled\n",procno);
    }
  }

  MPI_Finalize();
  return 0;
}
