#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  int mytid,ntids, p;
  double myvalue=1.,rvalue;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  for (p=0; p<ntids; p++) {
    if (p!=mytid)
      MPI_Send(&myvalue,1,MPI_DOUBLE,
	       /* to */ p,/* tag */ 0,comm);
  }
  for (p=ntids-1; p>=0; p--) {
    if (p!=mytid)
      MPI_Recv(&rvalue,1,MPI_DOUBLE,
	       /* from */ p,/* tag */ 0,comm,MPI_STATUS_IGNORE);
  }

  MPI_Finalize();
  return 0;
}
