#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int mytid, other, *recvbuf, *sendbuf;
  MPI_Comm comm;
  MPI_Status status;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm,&mytid);
  if (mytid>1) goto skip;
  for (int size=1; size<2000000000; size*=10) {
    sendbuf = (int*) malloc(size*sizeof(int));
    recvbuf = (int*) malloc(size*sizeof(int));
    if (!sendbuf || !recvbuf) {
      printf("Out of memory\n"); MPI_Abort(comm,1);
    }
    MPI_Send(sendbuf,size,MPI_INT,other,0,comm);
    MPI_Recv(recvbuf,size,MPI_INT,other,0,comm,&status);
    if (mytid==0)
      printf("Send did not block for size %d\n",size);
    free(sendbuf); free(recvbuf);
  }
  
 skip:
  MPI_Finalize();
  return 0;
}
