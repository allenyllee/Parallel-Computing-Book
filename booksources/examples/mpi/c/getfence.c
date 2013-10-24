/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013
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
    MPI_Win the_window;
    int my_number, other_number[2], other = ntids-1;
    if (mytid==other)
      other_number[1] = 27;
    MPI_Win_create(&other_number,2*sizeof(int),sizeof(int),
                   MPI_INFO_NULL,comm,&the_window);
    MPI_Win_fence(0,the_window);
    if (mytid==0) {
      MPI_Get( /* data on origin: */   &my_number, 1,MPI_INT,
	       /* data on target: */   other,1,    1,MPI_INT,
	       the_window);
    }
    MPI_Win_fence(0,the_window);
    if (mytid==0)
      printf("I got the following: %d\n",my_number);
    MPI_Win_free(&the_window);
  }

  MPI_Finalize();
  return 0;
}
