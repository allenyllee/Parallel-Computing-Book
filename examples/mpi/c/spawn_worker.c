/****************************************************************
 ****************************************************************
 ****
 **** This program file is part of the book and course
 **** "Parallel Computing"
 **** by Victor Eijkhout, copyright 2013-6
 ****
 **** spawn_worker.c : worker code for spawn example
 ****
 ****************************************************************
 ****************************************************************
 */


#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
#include "globalinit.c"

  int remotesize,nworkers,workerno;
  MPI_Comm parent;

  MPI_Comm_size(MPI_COMM_WORLD,&nworkers);
  MPI_Comm_rank(MPI_COMM_WORLD,&workerno);
  MPI_Comm_get_parent(&parent);
  ASSERTm(parent!=MPI_COMM_NULL,"No parent!");

  MPI_Comm_remote_size(parent, &remotesize);
  if (workerno==0) {
    printf("Deducing %d workers and %d parents\n",nworkers,remotesize);
  }
  //  ASSERTm(nworkers==size-1,"nworkers mismatch. probably misunderstanding");

  /* 
   * Parallel code here.  
   * The manager is represented as the process with rank 0 in (the remote 
   * group of) MPI_COMM_PARENT.  If the workers need to communicate among 
   * themselves, they can use MPI_COMM_WORLD. 
   */

  char hostname[256]; int namelen = 256;
  MPI_Get_processor_name(hostname,&namelen);
  printf("worker %d running on %s\n",workerno,hostname);

  MPI_Finalize();
  return 0;
}
