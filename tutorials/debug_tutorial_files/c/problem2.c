#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  MPI_Request *request;
  int mytid,ntids, root,p;
  double myvalue=1.;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  root = ntids-1;

  MPI_Reduce( &myvalue,&myvalue,1,MPI_DOUBLE,MPI_SUM,root,comm);
  if (mytid==root)
    printf("Reduction to %e\n",myvalue);

  MPI_Finalize();
  return 0;
}
