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
  int localsize =  10+10*( (float) rand()/(float)RAND_MAX - .5),
    root = ntids-1;

  int *localsizes=NULL,*offsets=NULL,*localdata=NULL,*alldata=NULL;
  // create local data
  localdata = (int*) malloc( localsize*sizeof(int) );
  for (i=0; i<localsize; i++)
    localdata[i] = mytid+1;
  // we assume that each process has an array "localdata"
  // of size "localsize"

  // the root process decides how much data will be coming:
  // allocate arrays to contain size and offset information
  if (mytid==root) {
    localsizes = (int*) malloc( (ntids+1)*sizeof(int) );
    offsets = (int*) malloc( ntids*sizeof(int) );
  }
  // everyone contributes their info
  MPI_Gather(&localsize,1,MPI_INT,
	     localsizes,1,MPI_INT,root,comm);
  if (mytid==root) {
    printf("Local sizes: ");
    for (i=0; i<ntids; i++)
      printf("%d, ",localsizes[i]);
    printf("\n");
  }
  // the root constructs the offsets array
  if (mytid==root) {
    offsets[0] = 0;
    for (i=0; i<ntids; i++)
      offsets[i+1] = offsets[i]+localsizes[i];
    alldata = (int*) malloc( offsets[ntids]*sizeof(int) );
  }
  // everyone contributes their data
  MPI_Gatherv(localdata,localsize,MPI_INT,
	      alldata,localsizes,offsets,MPI_INT,root,comm);
  if (mytid==root) {
    int p=0;
    printf("Collected:\n");
    for (i=0; i<ntids; i++) {
      int j;
      printf("  %d:",i);
      for (j=0; j<localsizes[i]-1; j++)
	printf("%d,",alldata[p++]);
      j=localsizes[i]-1;
      printf("%d;\n",alldata[p++]);
    }
  }

  MPI_Finalize();
  return 0;
}
