#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv) {
  int ntids,mytid;
  int token=1;
  MPI_Status status;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&ntids);
  MPI_Comm_rank(MPI_COMM_WORLD,&mytid);
  
  if (mytid>0) {
    MPI_Recv((void*)&token,1,MPI_INT,mytid-1,0,MPI_COMM_WORLD,
	     &status);
  }
  printf("Hello, this is processor %d out of %d\n",mytid,ntids);
  if (mytid<ntids-1) {
    MPI_Send((void*)&token,1,MPI_INT,mytid+1,0,MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
