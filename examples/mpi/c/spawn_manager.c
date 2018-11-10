/****************************************************************
 ****************************************************************
 ****
 **** This program file is part of the book and course
 **** "Parallel Computing"
 **** by Victor Eijkhout, copyright 2013-6
 ****
 **** spawn_manager.c : worker code for spawn example
 ****
 ****************************************************************
 ****************************************************************
 */

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
int main(int argc, char *argv[])
{

#define ASSERT(p) if (!(p)) {printf("Assertion failed for proc %d at line %d\n",procno,__LINE__); return -1;}
#define ASSERTm(p,m) if (!(p)) {printf("Message<<%s>> for proc %d at line %d\n",m,procno,__LINE__); return -1;}

  MPI_Comm comm;
  int procno=-1,nprocs,err;
  MPI_Init(&argc,&argv); 
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&procno); 
  MPI_Comm_size(comm,&nprocs); 
  MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN); 

  // Initialize the random number generator
  srand((int)(procno*(double)RAND_MAX/nprocs));

  int world_size,manager_rank, universe_size, *universe_sizep, flag;

  // this is in globalinit  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &manager_rank);

  MPI_Attr_get(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE,
	       (void*)&universe_sizep, &flag);
  if (!flag) {
    if (manager_rank==0) {
      printf("This MPI does not support UNIVERSE_SIZE.\nHow many processes total?");
      scanf("%d", &universe_size);
    }
    MPI_Bcast(&universe_size,1,MPI_INTEGER,0,MPI_COMM_WORLD);
  } else {
    universe_size = *universe_sizep;
    if (manager_rank==0)
      printf("Universe size deduced as %d\n",universe_size);
  }
  ASSERTm(universe_size>world_size,"No room to start workers");
  int nworkers = universe_size-world_size;

  /*  
   * Now spawn the workers. Note that there is a run-time determination 
   * of what type of worker to spawn, and presumably this calculation must 
   * be done at run time and cannot be calculated before starting 
   * the program. If everything is known when the application is  
   * first started, it is generally better to start them all at once 
   * in a single MPI_COMM_WORLD.  
   */

  const char *worker_program = "spawn_worker";
  int errorcodes[nworkers];
  MPI_Comm everyone;           /* intercommunicator */
  MPI_Comm_spawn(worker_program, MPI_ARGV_NULL, nworkers,
		 MPI_INFO_NULL, 0, MPI_COMM_WORLD, &everyone,
		 errorcodes);
  for (int ie=0; ie<nworkers; ie++)
    if (errorcodes[ie]!=0)
      printf("Error %d in spawning worker %d\n",errorcodes[ie],ie);  
  
  /* 
   * Parallel code here. The communicator "everyone" can be used 
   * to communicate with the spawned processes, which have ranks 0,.. 
   * MPI_UNIVERSE_SIZE-1 in the remote group of the intercommunicator 
   * "everyone". 
   */

  MPI_Finalize();
  return 0;
}

