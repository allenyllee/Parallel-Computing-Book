#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;
#include <mpi.h>

int main(int argc,char **argv) {
  int mytid;
  MPI_Comm comm;

  MPI_Init(&argc,&argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm,&mytid);


  {
    double tstart,tstop,jitter; 
    void *sendbuf;
    int wait;

    // wait for a random time, and measure how long
    wait = (int) ( 6.*rand() / (double)RAND_MAX );
    tstart = MPI_Wtime();
    sleep(wait);
    tstop = MPI_Wtime();
    jitter = tstop-tstart-wait;

    // find the maximum time over all processors
    if (mytid==0)
      sendbuf = MPI_IN_PLACE;
    else sendbuf = (void*)&jitter;
    MPI_Reduce(sendbuf,(void*)&jitter,1,MPI_DOUBLE,MPI_MAX,0,comm);
    if (mytid==0)
      printf("maximum OS jitter: %e\n",jitter);
  }

  MPI_Finalize();
  return 0;
}
