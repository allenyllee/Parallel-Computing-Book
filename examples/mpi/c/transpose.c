/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-9
   %%%%
   %%%% transpose.c : subarray datatype
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char **argv) {

#define MOD(i,n) (i+n)%(n)
#define ABS(x) ((x)<0 ? (-x) : (x))
#define MAX(x,y) ( (x)>(y) ? (x) : (y) )

#define MALLOC( t,v,n ) \
  t *v = (t*) malloc ( (n)*sizeof(t) ); \
  if (!v) { printf("Allocation failed in line %d\n",__LINE__); MPI_Abort(comm,0); }

  MPI_Comm comm;
  int procno=-1,nprocs,err;
  MPI_Init(&argc,&argv); 
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno); 
  MPI_Comm_size(comm,&nprocs); 
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN); 

  /*
   * Set up space component, 
   * we assume evenly divisible by nprocs
   */
  int nspace = 2*nprocs, local_space;
  local_space = nspace/nprocs;
  if ( nspace%nprocs != 0 ) {
    printf("For now, space has to be divisible by #procs\n"); MPI_Abort(comm,0); }
  int
    my_first_space = procno*local_space,
    my_last_space = (procno+1)*local_space - 1;

  /*
   * Set up time component, 
   * we assume evenly divisible by nprocs
   */
  int ntime = nprocs, local_time
  local_time = ntime/nprocs;
  if ( ntime%nprocs != 0 ) {
    printf("For now, time has to be divisible by #procs\n"); MPI_Abort(comm,0); }
  int
    my_first_time = procno*local_time,
    my_last_time = (procno+1)*local_time - 1;

  // Allocate vector suitable for space FFT
  MALLOC( double, gathered_space, local_space*local_time );
  /* double *gathered_space = (double*) malloc( local_space * sizeof(double) ); */
  /* if (!gathered_space) { */
  /*   printf("Space allocation failed\n"); MPI_Abort(comm,0); } */

  // Allocate vector suitable for time smoothing
  MALLOC( double, gathered_time, local_space*local_time );
  /* double *gathered_time = (double*) malloc( local_time * sizeof(double) ); */
  /* if (!gathered_time) { */
  /*   printf("Time allocation failed\n"); MPI_Abort(comm,0); } */

  {
    /*
     * Forward transpose from 
     *   redundant space / partitioned time
     * to
     *   redundant time / partitioned space
     */
    MALLOC( MPI_Datatype, send_space, nprocs );
    MALLOC( MPI_Datatype, recv_time, nprocs );
    MALLOC( MPI_Request, send_requests, nprocs );
    MALLOC( MPI_Request, recv_requests, nprocs );
      
    for (int p=0; p<nprocs; p++) {
      int
	first_space = procno*local_space,
	last_space = (procno+1)*local_space - 1;
      int
	first_time = procno*local_time,
	last_time = (procno+1)*local_time - 1;

      // describe the time slice that I get from process p
      MPI_Type_contiguous
	( local_space * local_time, MPI_DOUBLE, recv_time+p );
      MPI_Type_commit( recv_time[p] );
      MPI_Irecv( gathered_time+procno*local_space*local_time,
		 local_space*local_time,MPI_DOUBLE,
		 p,0, comm, revc_request+p );

      // describe the space slice that I send to process p
      MPI_Type_vector
	( send_space+p );
      MPI_Type_commit( send_space[p] );
    }
  }

  {
    /*
     * Backward transpose from 
     *   partitioned time / redundant space
     * to
     *   partitioned space / redundant time
     */
    MPI_Datatype *send_time =
      (MPI_Datatype) malloc ( nprocs * sizeof(MPI_Datatype) );
    if (!send_time) {
      printf("Time types allocation failed\n"); MPI_Abort(comm,0); }

    MPI_Datatype *recv_space =
      (MPI_Datatype) malloc ( nprocs * sizeof(MPI_Datatype) );
    if (!recv_space) {
      printf("Space types allocation failed\n"); MPI_Abort(comm,0); }
  }


  MPI_Datatype subarraytype;
  if (procno==sender) {
    MPI_Type_create_subarray
      (2,sizes,subsizes,starts,MPI_ORDER_C,MPI_DOUBLE,&subarraytype);
    MPI_Type_commit(&subarraytype);
    MPI_Send(source,1,subarraytype,the_other,0,comm);
  } else if (procno==receiver) {
    MPI_Status recv_status;
    int recv_count;
    MPI_Recv(target,count,MPI_DOUBLE,the_other,0,comm,
      &recv_status);
    MPI_Get_count(&recv_status,MPI_DOUBLE,&recv_count);
    ASSERT(recv_count==count);
  }
  
  if (procno==sender) {
    MPI_Aint true_lb,true_extent;
    MPI_Type_get_true_extent
      (subarraytype,&true_lb,&true_extent);
    printf("Found lb=%ld, extent=%ld\n",true_lb,true_extent);
    MPI_Aint
      comp_lb = ( starts[0]*sizes[1]+starts[1] )*sizeof(double),
      comp_extent = ( (starts[0]+subsizes[0]-1)*sizes[1] + starts[1]+subsizes[1] )*sizeof(double) - comp_lb;
    printf("Computing lb=%ld extent=%ld\n",comp_lb,comp_extent);
    ASSERTm(true_lb==comp_lb,"true lb wrong");
    ASSERTm(true_extent==comp_extent,"true extent wrong");
    MPI_Type_free(&subarraytype);
  }

  if (procno==receiver) {
    printf("received:");
    for (int i=0; i<count; i++)
      printf(" %6.3f",target[i]);
    printf("\n");
    int icnt = 0;
    for (int i=starts[0]; i<starts[0]+subsizes[0]; i++) {
      for (int j=starts[1]; j<starts[1]+subsizes[1]; j++) {
	printf("%d,%d\n",i,j);
	ASSERTm(icnt<count,"icnt too hight");
	int isrc = i*sizes[1]+j;
	if (source[isrc]!=target[icnt])
	  printf("target location (%d,%d)->%d %e s/b %e\n",i,j,icnt,target[icnt],source[isrc]);
	icnt ++;
      }
    }
  }

  if (procno==0)
    printf("Finished\n");

  /* MPI_Finalize(); */
  return 0;
}
