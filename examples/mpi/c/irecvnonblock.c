/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%% irecvnonblock.c : non-blocking exchange of data
   %%%%     between two processes
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int mytid,ntids, other, recvbuf, sendbuf;
  MPI_Comm comm;
  MPI_Request request[2];
  MPI_Status status[2];

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  recvbuf = -1; sendbuf = mytid+ntids;
  other = ntids-mytid;
  if (mytid==other) goto skip;
  MPI_Irecv(&recvbuf,1,MPI_INT,other,0,comm,&(request[0]));
  MPI_Isend(&sendbuf,1,MPI_INT,other,0,comm,&(request[1]));
  MPI_Waitall(2,request,status);
  
  if (recvbuf!=ntids+other)
    printf("[%d] Receive mismatch: %d s/b %d\n",mytid,recvbuf,ntids+other);

 skip:
  MPI_Finalize();
  return 0;
}
