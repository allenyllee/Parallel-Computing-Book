#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc,char **argv) {
  MPI_Comm comm;
  MPI_Request *request;
  int mytid,ntids, p;
  double myvalue=1.;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);
  request = (MPI_Request*) malloc(ntids*sizeof(MPI_Request));

  for (p=0; p<ntids; p++) {
    MPI_Isend(&myvalue,1,MPI_DOUBLE,
	      /* to */ p,/* tag */ 0,comm,request+p);
  }
  for (p=0; p<ntids; p++) {
    MPI_Recv(&myvalue,1,MPI_DOUBLE,
	     /* from */ p,/* tag */ 0,comm,MPI_STATUS_IGNORE);
  }

  MPI_Finalize();
  return 0;
}
