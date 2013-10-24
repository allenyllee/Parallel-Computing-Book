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

#include "globalinit.c"

  if (ntids<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }
  int sender = 0, receiver = 1, the_other = 1-mytid,
    length = 5,stride=2,block=1;
  double *source,*target;
  source = (double*) malloc(stride*length*sizeof(double));
  target = (double*) malloc(length*sizeof(double));

  for (i=0; i<stride*length; i++)
    source[i] = i+.5;

  MPI_Datatype newvectortype;
  MPI_Type_vector(length,block,stride,MPI_DOUBLE,&newvectortype);
  MPI_Type_commit(&newvectortype);
  if (mytid==sender) {
    MPI_Send(source,1,newvectortype,the_other,0,comm);
  } else if (mytid==receiver) {
    MPI_Status recv_status;
    int count;
    MPI_Recv(target,length,MPI_DOUBLE,the_other,0,comm,
      &recv_status);
    MPI_Get_count(&recv_status,MPI_DOUBLE,&count);
    ASSERT(count==length);
  }
  MPI_Type_free(&newvectortype);
  
  for (i=0; i<length; i++)
    if (target[i]!=source[stride*i])
      printf("location %d %e s/b %e\n",i,target[i],source[stride*i]);

  MPI_Finalize();
  return 0;
}
