#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv) {
  int ntids,mytid;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&ntids);
  MPI_Comm_rank(MPI_COMM_WORLD,&mytid);
  printf("Hello, this is processor %d out of %d\n",mytid,ntids);
  MPI_Finalize();
  return 0;
}
