/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise for Isend/Irecv
 ****
 ****************************************************************/

#include <iostream>
#include <sstream>
using namespace std;
#include <mpi.h>

int main() {

  MPI_Comm comm = MPI_COMM_WORLD;
  int nprocs, procno;
  stringstream proctext;

  MPI_Init(0,0);

  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&procno);

  int mydata=1.,leftdata=0.,rightdata=0.;
  int sendto,recvfrom;
  MPI_Request requests[4];

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  //    once to get data from the left, once from the right
  // -- for first/last process use MPI_PROC_NULL

  // get data from the left: who are you communicating with?
/**** your code here ****/
  MPI_Isend(&mydata,1,MPI_INT, sendto,0, comm,
/**** your code here ****/
	    );
  MPI_Irecv(&leftdata,1,MPI_INT, recvfrom,0, comm,
/**** your code here ****/
	    );

  // get data from the right: who are you communicating with?
/**** your code here ****/
  MPI_Isend(&mydata,1,MPI_INT, sendto,0, comm,
/**** your code here ****/
	    );
  MPI_Irecv(&rightdata,1,MPI_INT, recvfrom,0, comm,
/**** your code here ****/
	    );

  //
  // Now make sure all Isend/Irecv operations are completed
  //
/**** your code here ****/
  
  // check correctness
  mydata = mydata+leftdata+rightdata;
  if (procno==0 || procno==nprocs-1) {
    if (mydata!=2) {
      proctext << "Data on proc " << procno << " should be 2, not " << mydata << endl;
      cerr << proctext.str(); proctext.clear();
    }
  } else {
    if (mydata!=3) {
      proctext << "Data on proc " << procno << " should be 3, not " << mydata << endl;
      cerr << proctext.str(); proctext.clear();
    }
  }
  if (procno==0) {
    proctext << "Finished" << endl;
    cerr << proctext.str(); proctext.clear();
  }

  MPI_Finalize();
  return 0;
}
