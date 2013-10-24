/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013
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

#include "globalinit.c"
  int ierr;

  // Initialize the random number generator
  srand((int)(mytid*(double)RAND_MAX/ntids));

  if (mytid==ntids-1) {
    int *recv_buffer;
    MPI_Status status;

    recv_buffer = (int*) malloc((ntids-1)*sizeof(int));

    for (int p=0; p<ntids-1; p++) {
      ierr = MPI_Recv(recv_buffer+p,1,MPI_INT, MPI_ANY_SOURCE,0,comm,
		      &status); CHK(ierr);
      int sender = status.MPI_SOURCE;
      printf("Message from sender=%d: %d\n",
	     sender,recv_buffer[p]);
    }
  } else {
    float randomfraction = (rand() / (double)RAND_MAX);
    int randomwait = (int) ( ntids * randomfraction );
    printf("process %d waits for %e/%d=%d\n",
	   mytid,randomfraction,ntids,randomwait);
    sleep(randomwait);
    ierr = MPI_Send(&randomwait,1,MPI_INT, ntids-1,0,comm); CHK(ierr);
  }

  MPI_Finalize();
  return 0;
}
