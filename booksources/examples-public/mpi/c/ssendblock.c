#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int mytid, other, size, *recvbuf, *sendbuf;
  MPI_Comm comm;
  MPI_Status status;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm,&mytid);
  if (mytid>1) goto skip;
  sendbuf = (int*) malloc(sizeof(int));
  recvbuf = (int*) malloc(sizeof(int));
  size = 1;
  MPI_Ssend(sendbuf,size,MPI_INT,other,0,comm);
  MPI_Recv(recvbuf,size,MPI_INT,other,0,comm,&status);
  printf("This statement is not reached\n");
  free(sendbuf); free(recvbuf);
  
 skip:
  MPI_Finalize();
  return 0;
}
