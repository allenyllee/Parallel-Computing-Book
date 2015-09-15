/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
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

#define NEXPERIMENTS 10
#define NSIZES 6
  int src = 0,tgt = ntids-1,maxsize=1000000;
  double t[NSIZES], send[maxsize],recv[maxsize];

  MPI_Request requests[2];

  // First ordinary communication
  for (int cnt=0,s=1; cnt<NSIZES && s<maxsize; s*=10,cnt++) {
    if (mytid==src) {
      printf("Size %d\n",s);
      t[cnt] = MPI_Wtime();
      for (int n=0; n<NEXPERIMENTS; n++) {
	MPI_Isend(send,s,MPI_DOUBLE,tgt,0,comm,requests+0);
	MPI_Irecv(recv,s,MPI_DOUBLE,tgt,0,comm,requests+1);
	MPI_Waitall(2,requests,MPI_STATUSES_IGNORE);
      }
      t[cnt] = MPI_Wtime()-t[cnt];
    } else if (mytid==tgt) {
      for (int n=0; n<NEXPERIMENTS; n++) {
	MPI_Recv(recv,s,MPI_DOUBLE,src,0,comm,MPI_STATUS_IGNORE);
	MPI_Send(recv,s,MPI_DOUBLE,src,0,comm);
      }
    }
  }
  if (mytid==src) {
    for (int cnt=0,s=1; cnt<NSIZES; s*=10,cnt++) {
      t[cnt] /= NEXPERIMENTS;
      printf("Time for pingpong of size %d: %e\n",s,t[cnt]);
    }
  }

  // Now persistent communication
  for (int cnt=0,s=1; cnt<NSIZES; s*=10,cnt++) {
    if (mytid==src) {
      MPI_Send_init(send,s,MPI_DOUBLE,tgt,0,comm,requests+0);
      MPI_Recv_init(recv,s,MPI_DOUBLE,tgt,0,comm,requests+1);
      printf("Size %d\n",s);
      t[cnt] = MPI_Wtime();
      for (int n=0; n<NEXPERIMENTS; n++) {
	MPI_Startall(2,requests);
	MPI_Waitall(2,requests,MPI_STATUSES_IGNORE);
      }
      t[cnt] = MPI_Wtime()-t[cnt];
      MPI_Request_free(requests+0); MPI_Request_free(requests+1);
    } else if (mytid==tgt) {
      for (int n=0; n<NEXPERIMENTS; n++) {
	MPI_Recv(recv,s,MPI_DOUBLE,src,0,comm,MPI_STATUS_IGNORE);
	MPI_Send(recv,s,MPI_DOUBLE,src,0,comm);
      }
    }
  }
  if (mytid==src) {
    for (int cnt=0,s=1; cnt<NSIZES; s*=10,cnt++) {
      t[cnt] /= NEXPERIMENTS;
      printf("Time for persistent pingpong of size %d: %e\n",s,t[cnt]);
    }
  }

  MPI_Finalize();
  return 0;
}
