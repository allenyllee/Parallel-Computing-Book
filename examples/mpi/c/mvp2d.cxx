/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-6
   %%%%
   %%%% mvp2d.cxx : use Reduce_scatter and lots of group stuff
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "blis.h"
#include "tools.h"

// void cblas_dgemv (const enum CBLAS_ORDER order, const enum CBLAS_TRANSPOSE TransA, const int M, const int N, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)

class parallel_environment {
private:
  MPI_Comm global_comm,transpose_communicator;
public:
  MPI_Comm row_comm,col_comm;
private:
  void make_transpose_communicator() {
    MPI_Group global_group,transpose_group;
    int 
      *global_ranks = new int[ntids],
      *transpose_ranks = new int[ntids];

    {
      int loc;
      for (int loc=0; loc<ntids; loc++)
	global_ranks[loc] = loc;
      loc = 0;
      for (int col=0; col<ntids_j; col++)
	for (int row=0; row<ntids_i; row++)
	  transpose_ranks[loc++] = col+ntids_j*row;
    }
    MPI_Comm_group(global_comm,&global_group);
    MPI_Comm_group(global_comm,&transpose_group);
    int range_all[3] = {0,ntids-1,1}, **ranges_all;
    ranges_all = (int**) malloc( 1*sizeof(int*) );
    ranges_all[0] = range_all;
    MPI_Group_range_incl
      (global_group,1,(int(*)[3])ranges_all, &transpose_group);
    MPI_Group_translate_ranks
      (global_group,ntids,global_ranks,
       transpose_group,transpose_ranks);

    MPI_Group_free(&global_group); MPI_Group_free(&transpose_group);
    delete global_ranks; delete transpose_ranks;
    return;
  }
public:
  int ntids,mytid, ntids_i,ntids_j, row_number,col_number;
  parallel_environment( MPI_Comm world_comm, int ni, int nj ) {
    global_comm = world_comm; ntids_i = ni; ntids_j = nj;
    MPI_Comm_rank(global_comm,&mytid);
    MPI_Comm_size(global_comm,&ntids);
    if (ntids_i*ntids_j!=ntids)
      MPI_Abort(global_comm,
	 error(global_comm,0,
	       "pi x pj does not fit: %d x %d != %d",ntids_i,ntids_j,ntids));
    if (mytid==0) 
      printf("Setting up environment on %dx%d processors\n",ntids_i,ntids_j);
    make_transpose_communicator();
    row_number = ntids % ntids_i;
    col_number = ntids / ntids_j;
    MPI_Comm_split(global_comm,row_number,mytid,&row_comm);
    MPI_Comm_split(global_comm,col_number,mytid,&col_comm);
    return;
  }
};

class vector {
private:
  double local_value,*collect_in_row,*collect_in_col;
public:
  vector( parallel_environment environ ) {
    collect_in_row = new double[ environ.ntids_j ];
    collect_in_col = new double[ environ.ntids_i ];
    return;
  }
  void row_vector_bcast_from_zero( double *input ) {
    
    return;
  }
};

int main(int argc,char **argv) {
  int ntids;
  MPI_Comm comm;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm,&ntids);

  int
    ntids_i = commandline_argument(comm,argc,argv,"pi",1),
    ntids_j = commandline_argument(comm,argc,argv,"pj",ntids);

  parallel_environment environ = parallel_environment( comm,ntids_i,ntids_j );

  {
    int ione = 1, size_x = ntids_j,size_y = ntids_i;
    double
      my_x,my_y, one=1.e0, zero=0.e0,
      *local_x = new double[size_x],
      *local_y = new double[size_y],
      *local_matrix = new double[size_x*size_y];
    MPI_Allgather(&my_x,1,MPI_DOUBLE,
       	  local_x,1,MPI_DOUBLE,environ.col_comm);
    bli_dgemv( BLIS_NO_TRANSPOSE,
               BLIS_NO_CONJUGATE,
               size_y, size_x,
    	       &one,
               local_matrix, 1, size_y,
    	       local_x, 1,
	       &zero,
	       local_y, 1 );
    // blas_dgemv(CblasColMajor,CblasNoTrans,
    // 		size_y,size_x,1.e0,
    // 		local_matrix,size_y,
    // 		local_x,1,0.e0,local_y,1);
    MPI_Reduce_scatter(local_y,&my_y,&ione,MPI_DOUBLE,
	  MPI_SUM,environ.row_comm);
  }

  MPI_Finalize();
  return 0;
}
