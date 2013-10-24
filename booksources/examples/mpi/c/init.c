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
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {
  int input_argument;

#include "globalinit.c"

  MPI_Comm_rank(comm,&mytid);
  if (mytid==0) {
    if ( argc==1 || // the program is called without parameter
         ( argc>1 && !strcmp(argv[1],"-h") ) // user asked for help
         ) {
      printf("\nUsage: init [0-9]+\n");
      MPI_Abort(comm,1);
    }
    input_argument = atoi(argv[1]);
  }
  MPI_Bcast(&input_argument,1,MPI_INT,0,comm);

  MPI_Finalize();
  return 0;
}
