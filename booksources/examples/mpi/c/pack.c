/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% pack.c : use pack and unpack to send data
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
  if (argc<2) {
    printf("Usage: %s nnn\n",argv[0]);
    return -2;
  }
  int sender=0,receiver=ntids-1,other=ntids-1-mytid;
  int nsends=atoi(argv[1]),maxsends=100;
  if (nsends>maxsends) {
    printf("Max value for commandline argument: %d\n",maxsends);
    return -3;
  }
  int buflen = 1000,
    position;
  char *buffer = malloc(buflen);
  //snippet packunpack
  if (mytid==sender) {
  //snippet end
    int len=2*sizeof(int)+nsends*sizeof(double);
    if (len>buflen) {
      printf("Not enough buffer space, need %d\n",len);
      return -4;
    }
  //snippet packunpack
    MPI_Pack(&nsends,1,MPI_INT,buffer,buflen,&position,comm);
    for (i=0; i<nsends; i++) {
      double value = rand()/(double)RAND_MAX;
      MPI_Pack(&value,1,MPI_DOUBLE,buffer,buflen,&position,comm);
    }
    MPI_Pack(&nsends,1,MPI_INT,buffer,buflen,&position,comm);
    MPI_Send(buffer,position,MPI_PACKED,other,0,comm);
  } else if (mytid==receiver) {
    int irecv_value;
    double xrecv_value;
    MPI_Recv(buffer,buflen,MPI_PACKED,other,0,comm,MPI_STATUS_IGNORE);
    MPI_Unpack(buffer,buflen,&position,&nsends,1,MPI_INT,comm);
    for (i=0; i<nsends; i++) {
      MPI_Unpack(buffer,buflen,&position,&xrecv_value,1,MPI_DOUBLE,comm);
    }
    MPI_Unpack(buffer,buflen,&position,&irecv_value,1,MPI_INT,comm);
    ASSERT(irecv_value==nsends);
  //snippet end
  //snippet packunpack
  }
  //snippet end

  MPI_Finalize();
  return 0;
}
