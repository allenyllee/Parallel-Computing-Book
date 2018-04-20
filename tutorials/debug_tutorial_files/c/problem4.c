#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  int mytid,ntids, root;
  double buffer[100];
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  root = ntids-1;

  MPI_Allreduce( MPI_IN_PLACE,buffer,100000,MPI_DOUBLE,MPI_SUM,comm);
  if (mytid==root)
    printf("Successfully dispatched\n");

  MPI_Finalize();
  return 0;
}
