/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% sendrecv.c : ring communication through sendrecv
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

/** This program uses MPI_Sendrecv to implement
    deadlock-free blocking send/recv behaviour.
*/
int main(int argc,char **argv) {

#include "globalinit.c"

  /* We use processors 0,1,2 to set up a ring*/
  int right,left, my_data, other_data, reference_data;
  if (ntids<3) {
    printf("This program needs at least 3 processes\n");
    goto skip;
  }
  if (mytid>2)
    goto skip;

  //snippet sendrecvring
  right = (mytid+1)%3; left = (mytid+2)%3;
  //snippet end
  my_data = 10*mytid;
  reference_data = 10*left;

  //snippet sendrecvring
  MPI_Sendrecv( &my_data,1,MPI_INTEGER, right,0,
		&other_data,1,MPI_INTEGER, left,0,
		comm,MPI_STATUS_IGNORE);
  //snippet end
  if (other_data!=reference_data)
    printf("Error on process %d: received %d from %d, expected %d\n",
	   mytid,other_data,left,reference_data);
  
 skip:
  MPI_Finalize();
  return 0;
}
