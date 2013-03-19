#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm;
  int ntids,mytid;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&ntids);
  MPI_Comm_rank(comm,&mytid);

  {
    MPI_Win the_window;
    int my_number = 27, other_number;

    MPI_Win_create(&other_number,1,sizeof(int),
                   MPI_INFO_NULL,comm,&the_window);
    MPI_Win_fence(0,the_window);
    if (mytid==0) {
      MPI_Put( /* data on origin: */   &my_number, 1,MPI_INT,
	       /* data on target: */   1,0,        1,MPI_INT,
	       the_window);
    }
    MPI_Win_fence(0,the_window);
    if (mytid==1)
      printf("I got the following: %d\n",other_number);
  }

  MPI_Finalize();
  return 0;
}
