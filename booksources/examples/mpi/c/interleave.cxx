/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013
   %%%%
   %%%% interleave.cxx : scattering a vector datatype
   %%%%
   %%%% NOTE this example does not work
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
  int 
    sender = 0, count = 5;
  int displs[ntids],counts[ntids];
  int *source,*target;

  target = new int[count];

  if (mytid==sender) {
    source = new int[count*ntids*ntids];
    for (int i=0; i<ntids*count; i++)
      source[i] = i;
    // this is for error detection; these elements should go unused
    for (int i=ntids*count; i<ntids*count*ntids; i++)
      source[i] = -37;
    for (int i=0; i<ntids; i++) {
      counts[i] = 1; displs[i] = i;
    }
  }

  MPI_Datatype vectortype;
  MPI_Type_vector(count,1,ntids,MPI_INT,&vectortype);
  MPI_Type_commit(&vectortype);
  MPI_Scatterv( source,counts,displs,vectortype, target,count,MPI_INT,sender,comm);

  for (int i=0; i<count; i++) {
    if ( target[i]!=ntids*i+mytid )
      printf("Mismatch: proc %d, elt %d is %d s/b %d\n",mytid,i,target[i],ntids*i+mytid);
  }

  if (mytid==0) {
    MPI_Type_free(&vectortype);
    printf("Finished\n");
  }

  MPI_Finalize();
  return 0;
}
