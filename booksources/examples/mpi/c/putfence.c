/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char **argv) {

#include "globalinit.c"

  {
    //snippet putblock
    MPI_Win the_window;
    //snippet end
    int my_number=0, window_data[2], other = ntids-1;
    if (mytid==0)
      my_number = 37;

    //snippet putblock
    MPI_Win_create(&window_data,2*sizeof(int),sizeof(int),
                   MPI_INFO_NULL,comm,&the_window);
    MPI_Win_fence(0,the_window);
    if (mytid==0) {
      MPI_Put( /* data on origin: */   &my_number, 1,MPI_INT,
	       /* data on target: */   other,1,    1,MPI_INT,
	       the_window);
    }
    MPI_Win_fence(0,the_window);
    //snippet end
    if (mytid==other)
      printf("I got the following: %d\n",window_data[1]);
    //snippet putblock
    MPI_Win_free(&the_window);
    //snippet end
  }

  MPI_Finalize();
  return 0;
}
