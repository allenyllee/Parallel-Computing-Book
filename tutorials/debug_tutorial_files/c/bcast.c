#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  int mytid,ntids, p, bufsize=100000;
  double buffer[bufsize];
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  MPI_Bcast( buffer,bufsize,MPI_DOUBLE,mytid,comm);

  MPI_Finalize();
  return 0;
}
