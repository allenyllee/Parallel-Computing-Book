/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int other, recvbuf=2, sendbuf=3;

#include "globalinit.c"
  MPI_Status status;

#pragma recvblock
  other = 1-mytid;
#pragma end
  if (mytid>1) goto skip;
#pragma recvblock
  MPI_Recv(&recvbuf,1,MPI_INT,other,0,comm,&status);
  MPI_Send(&sendbuf,1,MPI_INT,other,0,comm);
  printf("This statement will not be reached on %d\n",mytid);
#pragma end
  
 skip:
  MPI_Finalize();
  return 0;
}
