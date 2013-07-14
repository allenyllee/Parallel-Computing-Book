#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char **argv) {

#include "globalinit.c"

  {
    MPI_Win the_window;
    int my_number = 27, other_number, other = ntids-1;

    MPI_Win_create(&other_number,1*sizeof(int),sizeof(int),
                   MPI_INFO_NULL,comm,&the_window);
    MPI_Win_fence(0,the_window);
    if (mytid==0) {
      MPI_Put( /* data on origin: */   &my_number, 1,MPI_INT,
	       /* data on target: */   other,0,    1,MPI_INT,
	       the_window);
    }
    MPI_Win_fence(0,the_window);
    if (mytid==other)
      printf("I got the following: %d\n",other_number);
    MPI_Win_free(&the_window);
  }

  MPI_Finalize();
  return 0;
}
