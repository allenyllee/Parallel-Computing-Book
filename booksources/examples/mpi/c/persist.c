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
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

#define NEXPERIMENTS 10
#define NSIZES 6
  int src = 0,tgt = ntids/2;
  double t[NSIZES], send[1000000],recv[1000000];


  MPI_Request requests[2];
  for (int cnt=0,s=1; cnt<NSIZES; s*=10,cnt++) {
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

  MPI_Finalize();
  return 0;
}
