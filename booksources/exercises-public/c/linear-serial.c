#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm;
  int ntids,mytid;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&ntids);
  MPI_Comm_rank(comm,&mytid);

  {
    MPI_Status status;
    int my_number = mytid, other_number=-1.;
    if (mytid<ntids-1)
      MPI_Ssend( /* data: */ &my_number,1,MPI_INT,
                 /* to: */ mytid+1, /* tag: */ 0, comm);
    if (mytid>0)
      MPI_Recv( /* data: */ &other_number,1,MPI_INT,
                /* from: */ mytid-1, 0, comm, &status);

    /* Correctness check */
    int *gather_buffer=NULL;
    if (mytid==0) {
      gather_buffer = (int*) malloc(ntids*sizeof(int));
      if (!gather_buffer) MPI_Abort(comm,1);
    }
    MPI_Gather(&other_number,1,MPI_INT,
               gather_buffer,1,MPI_INT, 0,comm);
    if (mytid==0) {
      int i,error=0;
      for (i=0; i<ntids; i++) 
        if (gather_buffer[i]!=i-1) {
          printf("Processor %d was incorrect: %d should be %d\n",
                 i,gather_buffer[i],i-1);
          error =1;
        }
      if (!error) printf("Success!\n");
      free(gather_buffer);
    }
  }

  MPI_Finalize();
  return 0;
}
