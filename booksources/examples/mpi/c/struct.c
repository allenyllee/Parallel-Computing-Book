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

  if (ntids<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }
  int sender = 0, receiver = 1, the_other = 1-mytid;
  struct object {
    char c;
    double x[2];
    int i;
  };

  if (mytid==sender)
    printf("Structure has size %d, naive size %d\n",
	   sizeof(struct object),
	   sizeof(char)+2*sizeof(double)+sizeof(int));
  struct object myobject;
  if (mytid==sender) {
    myobject.c = 'x'; myobject.x[0] = 2.7;
    myobject.x[1] = 1.5; myobject.i = 37;
  }

  MPI_Datatype newstructuretype;
  int structlen = 3;
  int blocklengths[structlen]; MPI_Datatype types[structlen];
  MPI_Aint displacements[structlen];
  // where are the components relative to the structure?
  blocklengths[0] = 1; types[0] = MPI_CHAR;
  displacements[0] = (size_t)&(myobject.c) - (size_t)&myobject;
  blocklengths[1] = 2; types[1] = MPI_DOUBLE;
  displacements[1] = (size_t)&(myobject.x[0]) - (size_t)&myobject;
  blocklengths[2] = 1; types[2] = MPI_INT;
  displacements[2] = (size_t)&(myobject.i) - (size_t)&myobject;
  MPI_Type_create_struct(structlen,blocklengths,displacements,types,&newstructuretype);
  MPI_Type_commit(&newstructuretype);
  {
    MPI_Aint typesize;
    MPI_Type_extent(newstructuretype,&typesize);
    if (mytid==0) printf("Type extent: %d bytes\n",typesize);
  }
  if (mytid==sender) {
    MPI_Send(&myobject,1,newstructuretype,the_other,0,comm);
  } else if (mytid==receiver) {
    MPI_Recv(&myobject,1,newstructuretype,the_other,0,comm,MPI_STATUS_IGNORE);
  }
  MPI_Type_free(&newstructuretype);
  
  if (mytid==receiver) {
    printf("%c %e %e %d\n",myobject.c,myobject.x[0],myobject.x[1],myobject.i);
    ASSERT(myobject.x[1]==1.5);
    ASSERT(myobject.i==37);
  }

  if (mytid==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
