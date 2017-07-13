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

  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);
#define CHK(x) if (x) {						 \
    char errtxt[200]; int len=200;				 \
  MPI_Error_string(x,errtxt,&len);				 \
  printf("p=%d, line=%d, err=%d, %s\n",procno,__LINE__,x,errtxt); \
  return x;}

  // Initialize the random number generator
  srand((int)(procno*(double)RAND_MAX/nprocs));

  if (procno==nprocs-1) {
    MPI_Status status;
    ierr = MPI_Recv(dummy,0,MPI_INT, MPI_ANY_SOURCE,0,comm,
                    &status); CHK(ierr);
    first_tid = status.MPI_SOURCE;
    ierr = MPI_Bcast(&first_tid,1,MPI_INT, nprocs-1,comm); CHK(ierr);
    printf("first msg came from %d\n",first_tid);
  } else {
    float randomfraction = (rand() / (double)RAND_MAX);
    int randomwait = (int) ( nprocs * randomfraction );
    MPI_Request request;
    printf("process %d waits for %e/%d=%d\n",
	   procno,randomfraction,nprocs,randomwait);
    sleep(randomwait);
    ierr = MPI_Isend(dummy,0,MPI_INT, nprocs-1,0,comm,
                     &request); CHK(ierr);
    ierr = MPI_Bcast(&first_tid,1,MPI_INT, nprocs-1,comm
                    ); CHK(ierr);
    if (procno!=first_tid) {
      ierr = MPI_Cancel(&request); CHK(ierr);
    }
  }

  MPI_Finalize();
  return 0;
}
