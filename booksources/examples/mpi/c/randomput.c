/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing for Science and Engineering"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% randomput : use of fences for synchronization
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

  if (ntids<3) {
    printf("Need at least 3 procs\n");
    MPI_Abort(comm,0);
  }

  {
    MPI_Win the_window;
    int my_number=1, my_sum = 0, window_data, other;

    //snippet randomput
    MPI_Win_create(&window_data,sizeof(int),sizeof(int),
                   MPI_INFO_NULL,comm,&the_window);

    for (int c=0; c<10; c++) {
      float randomfraction = (rand() / (double)RAND_MAX);
      if (randomfraction>.5)
        other = 2;
      else other = 1;
      window_data = 0;
      //snippet end
      MPI_Win_fence(0,the_window);
      if (mytid==0) {
	MPI_Put( /* data on origin: */   &my_number, 1,MPI_INT,
		 /* data on target: */   other,0,    1,MPI_INT,
		 the_window);
      }
      MPI_Win_fence(0,the_window);
      //snippet randomput
      my_sum += window_data;
    }

    if (mytid>0 && mytid<3)
      printf("Sum on %d: %d\n",mytid,my_sum);
    if (mytid==0) printf("(sum should be 10)\n");
    //snippet end

    MPI_Win_free(&the_window);
  }

  MPI_Finalize();
  return 0;
}
