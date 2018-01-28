/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-7
   %%%%
   %%%% graph.c : illustrating a graph topology
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

  if (nprocs<4) {
    printf("This program needs at least four processes\n");
    return -1;
  }

  if (procno==0) {
    int world_topo_status;
    MPI_Topo_test(comm,&world_topo_status);
    switch (world_topo_status) {
    case MPI_UNDEFINED :
      printf("Original topology undefined\n"); break;
    case MPI_CART :
      printf("Original topology cart\n"); break;
    case MPI_GRAPH :
      printf("Original topology graph\n"); break;
    case MPI_DIST_GRAPH :
      printf("Original topology dist_graph\n"); break;
    default:
      printf("Original topology is weird\n");
    }
  }

  //
  int idim,jdim;
  int ndim,periodic[2],dimensions[2],coord_2d[2],rank_2d;
  for (idim=(int)(sqrt(1.*nprocs)); idim>=2; idim--) {
    jdim = nprocs/idim;
    if (idim*jdim==nprocs) goto found;
  }
  printf("No prime numbers please\n"); return -1;
  MPI_Comm comm2d;
 found:
  if (procno==0)
    printf("Processor grid: %dx%d\n",idim,jdim);

  int proci,procj;
  proci = procno/jdim;
  procj = procno%jdim;
  printf("[%d] in grid at ( %2d,%2d )\n",procno,proci,procj);

#define PROC(i,j,I,J) ( (i+I)%I )*J + (j+J)%J
  {
    int sources[1]={procno};
    int degrees[1]={4};
    int destinations[4]={
      PROC(proci+1,procj+0,idim,jdim),
      PROC(proci-1,procj+0,idim,jdim),
      PROC(proci+0,procj+1,idim,jdim),
      PROC(proci+0,procj-1,idim,jdim)
    };
    int weights[4] = {1,1,1,1};
    printf("[%d] neighbours: %d, %d, %d, %d\n",
	   procno,
	   destinations[0],destinations[1],destinations[2],destinations[3]);
    MPI_Dist_graph_create
      (comm,
       /* I specify just one proc: me */ 1,sources,degrees,destinations,weights,
       MPI_INFO_NULL,0,
       &comm2d
       );
    if (procno==0) {
      int world_topo_status;
      MPI_Topo_test(comm2d,&world_topo_status);
      switch (world_topo_status) {
      case MPI_UNDEFINED :
	printf("Grid topology undefined\n"); break;
      case MPI_CART :
	printf("Grid topology cart\n"); break;
      case MPI_GRAPH :
	printf("Grid topology graph\n"); break;
      case MPI_DIST_GRAPH :
	printf("Grid topology dist_graph\n"); break;
      default:
	printf("Grid topology is weird\n");
      }
    }
  }

  // statistics gathering
  {
    int indegree,outdegree,weighted;
    MPI_Dist_graph_neighbors_count(comm2d,&indegree,&outdegree,&weighted);
    printf("[%d = (%d,%d)] indegree=%d, outdegree=%d, weighted=%d\n",
	   procno,proci,procj, indegree,outdegree,weighted);
  }

  int my_i[1],other_i[4], my_j[1],other_j[4];
  my_i[0] = proci; my_j[0] = procj;
  MPI_Neighbor_allgather( my_i,1,MPI_INT, other_i,1,MPI_INT, comm2d );
  MPI_Neighbor_allgather( my_j,1,MPI_INT, other_j,1,MPI_INT, comm2d );

  printf("[%d = (%d,%d)] : (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
	 procno,proci,procj,
	 other_i[0],other_j[0],other_i[1],other_j[1],other_i[2],other_j[2],other_i[3],other_j[3]);

  MPI_Finalize();
  return 0;
}
