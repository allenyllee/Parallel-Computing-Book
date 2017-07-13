/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% anysource.c : receive from any source
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


  if (procno==nprocs-1) {
    int *recv_buffer;
    MPI_Status status;

    recv_buffer = (int*) malloc((nprocs-1)*sizeof(int));

    for (int p=0; p<nprocs-1; p++) {
      err = MPI_Recv(recv_buffer+p,1,MPI_INT, MPI_ANY_SOURCE,0,comm,
		      &status); CHK(err);
      int sender = status.MPI_SOURCE;
      printf("Message from sender=%d: %d\n",
	     sender,recv_buffer[p]);
    }
  } else {
    float randomfraction = (rand() / (double)RAND_MAX);
    int randomwait = (int) ( nprocs * randomfraction );
    printf("process %d waits for %e/%d=%d\n",
	   procno,randomfraction,nprocs,randomwait);
    sleep(randomwait);
    err = MPI_Send(&randomwait,1,MPI_INT, nprocs-1,0,comm); CHK(err);
  }

  MPI_Finalize();
  return 0;
}
