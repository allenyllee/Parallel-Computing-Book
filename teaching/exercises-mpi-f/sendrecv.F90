! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-7
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program SendRecv
  implicit none

#include <mpif.h>

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target, error,errors

  !! data for this exercise:
  integer :: mydata=1, leftdata=0, rightdata=0
  integer :: sendto,recvfrom
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  

  !! Exercise:
  !! set `sendto' and `recvfrom' twice:
  !! once to get data from the left, once from the right

  !! first get left neighbour data
  !!hint: sendto =
  !!hint: recvfrom =
!!!! your code here !!!!
  call MPI_Sendrecv( mydata,1,MPI_INT, sendto,0, &
       leftdata,1,MPI_INT, recvfrom,0, &
       comm,MPI_STATUS_IGNORE,ierr)

  !! then get right neighbour data
!!!! your code here !!!!
  call MPI_Sendrecv( mydata,1,MPI_INT, sendto,0, &
       rightdata,1,MPI_INT, recvfrom,0, &
       comm,MPI_STATUS_IGNORE,ierr)

  mydata = mydata+leftdata+rightdata
  error = nprocs;
  if (procno==0 .OR. procno==nprocs-1) then
     if (mydata/=2) then
        print *,"Data on process",procno,"should be 2, not",mydata
        error = procno
     end if
  else
     if (mydata/=3) then
        print *,"Data on process",procno,"should be 3, not",mydata
        error = procno
     end if
  end if
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm,ierr)
  if (procno==0) then
     if (errors==nprocs) then
        print *,"Finished; all results correct"
     else
        print *,"First error occurred on proc",errors
     end if
  end if

  call MPI_Finalize(ierr)
  
end Program SendRecv
