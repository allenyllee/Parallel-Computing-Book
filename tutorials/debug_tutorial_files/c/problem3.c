#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  int mytid,ntids, root;
  double buffer[1];
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  root = ntids-1;

  MPI_Bcast( buffer,100000,MPI_DOUBLE,root,comm);
  if (mytid==root)
    printf("Successfully dispatched\n");

  MPI_Finalize();
  return 0;
}
