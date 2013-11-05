/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
using namespace std;

int main(int argc,char **argv) {

#include "globalinit.c"

  // Create the table in fake shared memory
  MPI_Win the_window;
  int repository = 0, ninputs = ntids-1; //10*ntids;
  float *inputs;

  if (mytid==repository) {
    // Processor zero creates a table of inputs
    // and associates that with the window
    inputs = new float[ninputs];
    for (int i=0; i<ninputs; i++)
      inputs[i] = .01 * rand() / (float)RAND_MAX ;
    MPI_Win_create(inputs,ninputs*sizeof(float),sizeof(float),
		   MPI_INFO_NULL,comm,&the_window);
  } else {
    // for all other processors the window is null
    MPI_Win_create(NULL,0,sizeof(int),
		   MPI_INFO_NULL,comm,&the_window);
  }

  // bookkeeping: which jobs did I process?
  int *myjobs = new int[ninputs];
  for (int i=0; i<ninputs; i++) 
    myjobs[i] = 0;

  if (mytid!=repository) {

    float occupied=-1.,table_element;

    int loc=0;

      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,repository,0,the_window);

      cout << "locking from" << mytid << "\n";
      // read the table element by getting the result from adding zero
      err = MPI_Fetch_and_op(&occupied,&table_element,MPI_INT,
       	            repository,loc,MPI_REPLACE,the_window); CHK(err);
      MPI_Win_unlock(repository,the_window);

  }

  err = MPI_Win_free(&the_window); CHK(err);

  MPI_Finalize();
  return 0;
}
