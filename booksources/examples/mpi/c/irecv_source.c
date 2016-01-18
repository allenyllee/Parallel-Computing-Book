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
  int mytid,ntids, ierr;
  MPI_Comm comm;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN);
#define CHK(x) if (x) {						 \
    char errtxt[200]; int len=200;				 \
  MPI_Error_string(x,errtxt,&len);				 \
  printf("p=%d, line=%d, err=%d, %s\n",mytid,__LINE__,x,errtxt); \
  return x;}

  // Initialize the random number generator
  srand((int)(mytid*(double)RAND_MAX/ntids));

//snippet waitforany
  if (mytid==ntids-1) {
    int *recv_buffer;
    MPI_Request *request; MPI_Status status;
    recv_buffer = (int*) malloc((ntids-1)*sizeof(int));
    request = (MPI_Request*) malloc((ntids-1)*sizeof(MPI_Request));

    for (int p=0; p<ntids-1; p++) {
      ierr = MPI_Irecv(recv_buffer+p,1,MPI_INT, p,0,comm,
		      request+p); CHK(ierr);
    }
    for (int p=0; p<ntids-1; p++) {
      int index,sender;
      MPI_Waitany(ntids-1,request,&index,&status); //MPI_STATUS_IGNORE);
      if (index!=status.MPI_SOURCE)
	printf("Mismatch index %d vs source %d\n",index,status.MPI_SOURCE);
      printf("Message from %d: %d\n",index,recv_buffer[index]);
    }
//snippet end
  } else {
    float randomfraction = (rand() / (double)RAND_MAX);
    int randomwait = (int) ( 2* ntids * randomfraction );
    printf("process %d waits for %d\n",mytid,randomwait);
    sleep(randomwait);
    ierr = MPI_Send(&mytid,1,MPI_INT, ntids-1,0,comm); CHK(ierr);
  }
//snippet end

  MPI_Finalize();
  return 0;
}
