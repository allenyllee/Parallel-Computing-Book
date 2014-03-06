#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <mpi.h>

int main(int argc,char **argv) {
  int mytid;
  MPI_Comm comm;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);


  {
    double tstart,tstop,duration,x; 
    void *sendbuf;
    int max_iter;

    // do something random, and measure how long it takes
    max_iter = (int) ( 1000000.*rand() / (double)RAND_MAX );
    x = 999.*rand() / (double)RAND_MAX;
    tstart = MPI_Wtime();
    for (int i=0; i<max_iter; i++)
      x += sin(x);
    tstop = MPI_Wtime();
    duration = tstop-tstart;

    // find the maximum time over all processors
    if (mytid==0)
      sendbuf = MPI_IN_PLACE;
    else sendbuf = (void*)&duration;
    MPI_Reduce(sendbuf,(void*)&duration,1,MPI_DOUBLE,MPI_MAX,0,comm);
    if (mytid==0)
      printf("Average duration: %e, result=%e\n",duration,x);
  }

  MPI_Finalize();
  return 0;
}
