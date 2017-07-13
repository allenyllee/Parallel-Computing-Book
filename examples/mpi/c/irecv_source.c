/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% irecv_source.c : combine Irecv and Waitany
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
  int procno,nprocs, ierr;
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
    int *recv_buffer;
    MPI_Request *request; MPI_Status status;
    recv_buffer = (int*) malloc((nprocs-1)*sizeof(int));
    request = (MPI_Request*) malloc((nprocs-1)*sizeof(MPI_Request));

    for (int p=0; p<nprocs-1; p++) {
      ierr = MPI_Irecv(recv_buffer+p,1,MPI_INT, p,0,comm,
		      request+p); CHK(ierr);
    }
    for (int p=0; p<nprocs-1; p++) {
      int index,sender;
      MPI_Waitany(nprocs-1,request,&index,&status); //MPI_STATUS_IGNORE);
      if (index!=status.MPI_SOURCE)
	printf("Mismatch index %d vs source %d\n",index,status.MPI_SOURCE);
      printf("Message from %d: %d\n",index,recv_buffer[index]);
    }
  } else {
    float randomfraction = (rand() / (double)RAND_MAX);
    int randomwait = (int) ( 2* nprocs * randomfraction );
    printf("process %d waits for %d\n",procno,randomwait);
    sleep(randomwait);
    ierr = MPI_Send(&procno,1,MPI_INT, nprocs-1,0,comm); CHK(ierr);
  }

  MPI_Finalize();
  return 0;
}
