/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-8
 ****
 **** MPI Exercise for communicator splitting
 ****
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc,char **argv) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  
  // data specifically for this program
  int
    nrows,ncols,
    row_rank,col_rank;
  MPI_Comm row_comm,col_comm;

  MPI_Init(&argc,&argv);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  //
  // Try to arrange the processors in a grid
  //
  for (nrows=sqrt(nprocs+1); nrows>=1; nrows--)
    if (nprocs%nrows==0)
      break;
  if (nrows==1) {
    if (procno==0)
      printf("Number of processes is prime\n");
    MPI_Abort(comm,0);
  }
  ncols = nprocs/nrows;

  //
  // Exercise:
  // - compute the coordinates of this process
  //
  int row_no,col_no;
/**** your code here ****/

  //
  // Make a `col_comm' communicator with all processes in this column
  //
  MPI_Comm_split(comm,
/**** your code here ****/
		 );
  //
  // Make a `row_comm' communicator with all processes in this row
  //
  MPI_Comm_split(comm,
/**** your code here ****/
		 );

  //
  // Now find `row_rank': the number I am in my row
  // by using MPI_Comm_rank
  //
  MPI_Comm_rank(
/**** your code here ****/
		);
  //
  // also find `col_rank': the number I am in my column
  // by using MPI_Comm_rank
  //
  MPI_Comm_rank(
/**** your code here ****/
		);

  // correctness check
  int error=nprocs,errors;

  //
  // Now check that the rank in the row equals the column number
  //
  if (row_rank!=col_no) {
    printf("[%d=%d,%d] wrong row rank %d\n",
	   procno,row_no,col_no,row_rank);
    error = procno;
  }
  //
  // Now check that the rank in the column equals the row number
  //
  if (col_rank!=row_no) {
    printf("[%d=%d,%d] wrong col rank %d\n",
	   procno,row_no,col_no,col_rank);
    error = procno;
  }

  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      printf("Finished; all results correct\n");
    else
      printf("First error occurred on proc %d\n",errors);
  }
  
  MPI_Finalize();
  return 0;
}
