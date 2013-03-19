#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int mytid, other, recvbuf=2, sendbuf=3;
  MPI_Comm comm;
  MPI_Request request[2]
  MPI_Status status[2];

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm,&mytid);
  other = 1-mytid;
  if (mytid>1) goto skip;
  MPI_Irecv(&recvbuf,1,MPI_INT,other,0,comm,&(request[0]));
  MPI_Isend(&sendbuf,1,MPI_INT,other,0,comm,&(request[1]));
  MPI_Waitall(2,request,status);
  
 skip:
  MPI_Finalize();
  return 0;
}
