! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-7
!****
!**** MPI Exercise for Isend/Irecv
!****
!****************************************************************/

Program Isendirecv
  implicit none

#include <mpif.h>

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr
  integer :: source,target

  !! data for this exercise:
  integer :: mydata=1, leftdata=0, rightdata=0
  integer :: sendto,recvfrom
  integer :: requests(4)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  

  !! Exercise:
  !! set `sendto' and `recvfrom' twice:
  !! once to get data from the left, once from the right

  !!
  !! first get left neighbour data
  !!

  !! set processes to communicate with
!!!! your code here !!!!

  !! now do the Isend/Irecv calls
  call MPI_Isend(mydata, 1,MPI_INT, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(leftdata, 1,MPI_INT, recvfrom,0, comm, &
!!!! your code here !!!!
       ierr)

  !!
  !! then get right neighbour data
  !!

  !! set processors to communicate with
!!!! your code here !!!!

  !! now do the Isend/Irecv calls
  call MPI_Isend(mydata, 1,MPI_INT, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(rightdata, 1,MPI_INT, recvfrom,0, comm, &
!!!! your code here !!!!
       ierr)

  !!
  !! Now make sure all Irecv/Irecv operations are completed
  !!
!!!! your code here !!!!

  !!
  !! Check correctness
  !!
  mydata = mydata+leftdata+rightdata
  if (procno==0 .OR. procno==nprocs-1) then
     if (mydata/=2) print *,"Data on process",procno,"should be 2, not",mydata
  else
     if (mydata/=3) print *,"Data on process",procno,"should be 3, not",mydata
  end if
  if (procno==0) then
     print *,"Finished"
  end if

  call MPI_Finalize(ierr)
  
end Program Isendirecv

