#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int mytid, other, recvbuf=2, sendbuf=3;
  MPI_Comm comm;
  MPI_Status status;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm,&mytid);
  other = 1-mytid;
  if (mytid>1) goto skip;
  MPI_Recv(&recvbuf,1,MPI_INT,other,0,comm,&status);
  MPI_Send(&sendbuf,1,MPI_INT,other,0,comm);
  printf("This statement will not be reached on %d\n",mytid);
  
 skip:
  MPI_Finalize();
  return 0;
}
