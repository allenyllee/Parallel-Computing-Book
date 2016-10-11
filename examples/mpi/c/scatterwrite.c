/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% blockwrite.c : MPI IO example
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

  MPI_Comm comm;
  int nprocs,procno;
  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  // data for this program specifically
  int nwords = 1;
  int *output_data = (int*) malloc(nwords*sizeof(int));
  for (int iw=0; iw<nwords; iw++)
    output_data[iw] = procno*nwords+iw;
  
  MPI_File mpifile;
  MPI_File_open(comm,"blockwrite.dat",
		MPI_MODE_CREATE | MPI_MODE_RDWR,MPI_INFO_NULL,
		&mpifile);

  MPI_Datatype inmemory_type,infile_type;
  MPI_Type_contiguous(nwords,MPI_INT,&inmemory_type);
  MPI_Type_commit(&inmemory_type);
  MPI_Type_vector(nwords,1,nprocs,MPI_INT,&infile_type);
  MPI_Type_commit(&infile_type);

  /* MPI_File_set_view */
  /*   (mpifile, */
  /*    /\* displacement= *\/ procno,MPI_INT,infile_type, */
  /*    "native",MPI_INFO_NULL); */
  /* MPI_File_write */
  /*   (mpifile,output_data,1,inmemory_type,MPI_STATUS_IGNORE);      */
  if (procno==0)
  MPI_File_write_at
    (mpifile,procno*nwords*sizeof(int),output_data,nwords,MPI_INT,MPI_STATUS_IGNORE);     

  MPI_Type_free(&inmemory_type);
  MPI_Type_free(&infile_type);

  MPI_File_close(&mpifile);
  
  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}
