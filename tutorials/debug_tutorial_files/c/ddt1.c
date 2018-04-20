/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013/4/5
   %%%%
   %%%% ddt1.c : code with a parallelism bug
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

void loop_for_awhile(MPI_Comm comm) {
  int mytid,ntids, it;

  MPI_Comm_rank(comm,&mytid);
  MPI_Comm_size(comm,&ntids);

  // Initialize the random number generator
  srand((int)(mytid*(double)RAND_MAX/ntids));

  for (it=0; ; it++) {
    double randomnumber = ntids * ( rand() / (double)RAND_MAX );
    //printf("[%d] iteration %d, random %e\n",mytid,it,randomnumber);
    if (randomnumber>mytid && randomnumber<mytid+1./(ntids+1))
      MPI_Finalize();
    //MPI_Barrier(comm);
  }
}

int main(int argc,char **argv) {
  MPI_Comm comm;
  
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;

  loop_for_awhile(comm);

  MPI_Finalize();
  return 0;
}
