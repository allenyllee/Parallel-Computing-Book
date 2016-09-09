/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% commsplit.c : illustrating MPI_Comm_split
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

  int color;
  int new_mytid;
  MPI_Comm mod2comm,mod4comm;

  int mydata = mytid;
  // create sub communicator modulo 2
  color = mytid%2;
  MPI_Comm_split(MPI_COMM_WORLD,color,mytid,&mod2comm);
  MPI_Comm_rank(mod2comm,&new_mytid);

  // create sub communicator modulo 4 recursively
  color = new_mytid%2;
  MPI_Comm_split(mod2comm,color,new_mytid,&mod4comm);
  MPI_Comm_rank(mod4comm,&new_mytid);

  if (mydata/4!=new_mytid)
    printf("Error %d %d %d\n",mytid,new_mytid,mydata/4);

  if (mytid==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}

