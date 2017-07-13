/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% pingpong.c : ping-pong with blocking send and recv
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

#define NEXPERIMENTS 100
  int src = 0,tgt = nprocs/2;
  double t, send=1.1,recv;
  if (procno==src) {
    t = MPI_Wtime();
    for (int n=0; n<NEXPERIMENTS; n++) {
      MPI_Send(&send,1,MPI_DOUBLE,tgt,0,comm);
      MPI_Recv(&recv,1,MPI_DOUBLE,tgt,0,comm,MPI_STATUS_IGNORE);
    }
    t = MPI_Wtime()-t; t /= NEXPERIMENTS;
    printf("Time for pingpong: %e\n",t);
  } else if (procno==tgt) {
    for (int n=0; n<NEXPERIMENTS; n++) {
      MPI_Recv(&recv,1,MPI_DOUBLE,src,0,comm,MPI_STATUS_IGNORE);
      MPI_Send(&recv,1,MPI_DOUBLE,src,0,comm);
    }
  }

  MPI_Finalize();
  return 0;
}
