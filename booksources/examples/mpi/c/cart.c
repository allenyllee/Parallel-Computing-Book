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
#include <math.h>
#include "mpi.h"

int main(int argc,char **argv) {

#include "globalinit.c"

  if (ntids<4) {
    printf("This program needs at least four processes\n");
    return -1;
  }

  //
  int idim,jdim;
  int ndim,periodic[2],dimensions[2],coord_2d[2],rank_2d;
  for (idim=(int)(sqrt(1.*ntids)); idim>=2; idim--) {
    jdim = ntids/idim;
    if (idim*jdim==ntids) goto found;
  }
  printf("No prime numbers please\n"); return -1;
  MPI_Comm comm2d;
 found:

  ndim = 2; periodic[0] = periodic[1] = 0;
  dimensions[0] = idim; dimensions[1] = jdim;
  MPI_Cart_create(comm,ndim,dimensions,periodic,1,&comm2d);
  MPI_Cart_coords(comm2d,mytid,ndim,coord_2d);
  MPI_Cart_rank(comm2d,coord_2d,&rank_2d);
  printf("I am %d: (%d,%d); originally %d\n",rank_2d,coord_2d[0],coord_2d[1],mytid);

  int rank_left,rank_right,rank_up,rank_down;
  char indata[4]; int idata=0,sdata=0;
  for (i=0; i<4; i++)
    indata[i] = 32;
  char mychar = 65+mytid;
  MPI_Cart_shift(comm2d,0,+1,&rank_2d,&rank_right);
  MPI_Cart_shift(comm2d,0,-1,&rank_2d,&rank_left);
  MPI_Cart_shift(comm2d,1,+1,&rank_2d,&rank_up);
  MPI_Cart_shift(comm2d,1,-1,&rank_2d,&rank_down);
  int irequest = 0; MPI_Request *requests = malloc(8*sizeof(MPI_Request));
  MPI_Isend(&mychar,1,MPI_CHAR,rank_right, 0,comm, requests+irequest++);
  MPI_Isend(&mychar,1,MPI_CHAR,rank_left,  0,comm, requests+irequest++);
  MPI_Isend(&mychar,1,MPI_CHAR,rank_up,    0,comm, requests+irequest++);
  MPI_Isend(&mychar,1,MPI_CHAR,rank_down,  0,comm, requests+irequest++);
  MPI_Irecv( indata+idata++, 1,MPI_CHAR, rank_right, 0,comm, requests+irequest++);
  MPI_Irecv( indata+idata++, 1,MPI_CHAR, rank_left,  0,comm, requests+irequest++);
  MPI_Irecv( indata+idata++, 1,MPI_CHAR, rank_up,    0,comm, requests+irequest++);
  MPI_Irecv( indata+idata++, 1,MPI_CHAR, rank_down,  0,comm, requests+irequest++);
  MPI_Waitall(irequest,requests,MPI_STATUSES_IGNORE);
  printf("[%d] %s\n",mytid,indata);
  /* for (i=0; i<4; i++) */
  /*   sdata += indata[i]; */
  /* printf("[%d] %d,%d,%d,%d sum=%d\n",mytid,indata[0],indata[1],indata[2],indata[3],sdata); */
  if (mytid==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
