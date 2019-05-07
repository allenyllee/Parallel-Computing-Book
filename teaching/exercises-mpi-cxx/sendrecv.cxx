/****************************************************************
 ****
 **** This program file is part of the book 
 **** `Parallel programming with MPI and OpenMP'
 **** by Victor Eijkhout, eijkhout@tacc.utexas.edu
 ****
 **** copyright Victor Eijkhout 2012-9
 ****
 **** MPI Exercise for sendrecv : three-point combination
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

  int mydata=1,leftdata=0,rightdata=0;
  int sendto,recvfrom;

  // Exercise:
  // -- set `sendto' and `recvfrom' twice
  // -- once to get data from the left, once from the right

  // first get left neighbour data
  //hint:  sendto = 
  //hint:  recvfrom =
/**** your code here ****/
  MPI_Sendrecv
    (&mydata,1,MPI_INT, sendto,0,
     &leftdata,1,MPI_INT, recvfrom,0, comm,MPI_STATUS_IGNORE);

  // then the right neighbour data
  //hint:  recvfrom =
  //hint:  sendto = 
/**** your code here ****/
  MPI_Sendrecv
    (&mydata,1,MPI_INT, sendto,0,
     &rightdata,1,MPI_INT, recvfrom,0, comm,MPI_STATUS_IGNORE);

  // check correctness
  mydata = mydata+leftdata+rightdata;

  int error=nprocs,errors;
  if (procno==0 || procno==nprocs-1) {
    if (mydata!=2) {
      proctext << "Data on proc " << procno << " should be 2, not " << mydata << endl;
      error = procno;
    }
    cerr << proctext.str(); proctext.clear();
  } else {
    if (mydata!=3) {
      proctext << "Data on proc " << procno << " should be 3, not " << mydata << endl;
      error = procno;
    }
  }

  MPI_Allreduce(&error,&errors,1,MPI_INT,MPI_MIN,comm);
  if (procno==0) {
    if (errors==nprocs) 
      proctext << "Finished; all results correct" << endl;
    else
      proctext << "First error occurred on proc " << errors << endl;
    cerr << proctext.str(); proctext.clear();
  }

  MPI_Finalize();
  return 0;
}
