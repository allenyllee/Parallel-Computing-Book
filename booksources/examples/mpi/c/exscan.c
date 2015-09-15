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

#include "globalinit.c"

  int my_first=0,localsize;
  // localsize = ..... result of local computation ....
  localsize = 10+(int) (mytid*(1+ (float) rand()/(float)RAND_MAX ));
  // find myfirst location based on the local sizes
  err = MPI_Exscan(&localsize,&my_first,
              1,MPI_INT,MPI_SUM,comm); CHK(err);
  printf("[%d] localsize %d, first %d\n",mytid,localsize,my_first);

  MPI_Finalize();
  return 0;
}
