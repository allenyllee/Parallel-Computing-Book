/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-8
   %%%%
   %%%% jordancol.c : Gauss-Jordan with matrix distributed by columns
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc,char **argv) {

  MPI_Init(&argc,&argv);
  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs,procno;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  /*
   * We set the matrix size to the number of processes.
   * Each process allocates
   * - one column of the matrix
   * - a scaling vector
   * - redundantly, the solution vector and right-hand side
   */
  int N = nprocs;
  double
    matrix[N],
    solution[N], rhs[N], scalings[N];

  /*
   * Fill the matrix with random data;
   * let's assume that the matrix is non-singular
   * and we'll not need to pivot
   */
  // first set a unique random seed
  srand((int)(procno*(double)RAND_MAX/nprocs));
  
  /*
   * Fill the matrix with random data
   * and increase the diagonal for numerical stability
   */
  for (int row=0; row<N; row++) {
    matrix[row] = rand()/(double)RAND_MAX;
    if (row==procno)
      matrix[row] += .5;
  }
  
  // Disabled code for writing to file
  if (0) {
    double *global;
    if (procno==0) global = malloc(N*N*sizeof(double));
    MPI_Gather(matrix,N,MPI_DOUBLE,global,N,MPI_DOUBLE,0,comm);
    FILE *m; m = fopen("jordanmat.dat","w");
    if (procno==0) {
      fprintf(m,"Matrix of size: %d (by columns)\n",N);
      for (int el=0; el<N*N; el++)
	fprintf(m,"%e\n",global[el]);
      fclose(m);
    }
  }

  /*
   * Set the right-hand side to row sums,
   * so that the solution vector is identically one.
   * We use one long reduction.
   */
  MPI_Allreduce(matrix,rhs,N,MPI_DOUBLE,MPI_SUM,comm);
  //printf("rhs=["); for (int i=0; i<N; i++) printf("%e,",rhs[i]); printf("]\n");

  /*
   * Now iterate over the columns, 
   * using the diagonal element as pivot.
   */
  for (int piv=0; piv<N; piv++) {
    /*
     * If this proc owns the pivot, compute the scaling vector
     */
    if (piv==procno) {
      double pivot = matrix[piv];
      // scaling factors per row
      for (int row=0; row<N; row++)
	scalings[row] = matrix[row]/pivot;
    }
    /*
     * Exercise:
     * make sure that everyone knows the scaling factors
     */
/**** your code here ****/

    /*
     * Now update the matrix.
     * Answer for yourself: why is there no loop over the columns?
     */
    for (int row=0; row<N; row++) {
      if (row==piv) continue;
      matrix[row] =
	matrix[row] - scalings[row]*matrix[piv];
    }
    // update the right hand side
    for (int row=0; row<N; row++) {
      if (row==piv) continue;
      rhs[row] = rhs[row] - scalings[row]*rhs[piv];
    }
  }
  //printf("swept=["); for (int i=0; i<N; i++) printf("%e,",rhs[i]); printf("]\n");

  // check that we have swept
  for (int row=0; row<N; row++) {
    if (row==procno) continue;
    if (abs(matrix[row])>1.e-14)
      printf("Wrong value at [%d,%d]: %e\n",row,procno,matrix[row]);
  }
  //printf("Diagonal element %d: %e\n",procno,matrix[procno]);

  /*
   * Solve the system
   */
  double local_solution = rhs[procno]/matrix[procno];
  MPI_Allgather(&local_solution,1,MPI_DOUBLE,solution,1,MPI_DOUBLE,comm);

  // check correct solution
  if (procno==0) {
    int success=1;
    for (int row=0; row<N; row++)
      if ( abs(solution[row]-1.)>1.e-13 ) {
	printf("Wrong solution at [%d]: %e\n",row,solution[row]);
	success = 0;
      }
    if (success>0)
      printf("Success\n");
  }

  MPI_Finalize();

  return 0;
}
