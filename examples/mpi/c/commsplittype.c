/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% commsplittype.c : illustrating MPI_Comm_split
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  if (ntids<3) {
    printf("This program needs at least three processes\n");
    return -1;
  }

  int new_procno,new_nprocs;
  MPI_Comm sharedcomm;

  MPI_Comm_split(MPI_COMM_WORLD,MPI_COMM_TYPE_SHARED,mytid,&sharedcomm);
  MPI_Comm_rank(sharedcomm,&new_nprocs);
  MPI_Comm_rank(sharedcomm,&new_procno);

  ASSERT(new_procno<CORES_PER_NODE);    

  if (mytid==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}

