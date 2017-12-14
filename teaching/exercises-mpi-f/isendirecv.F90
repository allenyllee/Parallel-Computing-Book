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
  integer :: source,target, error,errors

  !! data for this exercise:
  real*8 :: mydata, leftdata=0, rightdata=0, check
  integer :: sendto,recvfrom
  integer :: requests(4)
 
  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)
  mydata = procno

  !! Exercise:
  !! set `sendto' and `recvfrom' twice:
  !! once to get data from the left, once from the right

  !!
  !! first get left neighbour data
  !!

  !! set processes to communicate with
!!!! your code here !!!!

  !! now do the Isend/Irecv calls
  call MPI_Isend(mydata, 1,MPI_REAL8, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(leftdata, 1,MPI_REAL8, recvfrom,0, comm, &
!!!! your code here !!!!
       ierr)

  !!
  !! then get right neighbour data
  !!

  !! set processors to communicate with
!!!! your code here !!!!

  !! now do the Isend/Irecv calls
  call MPI_Isend(mydata, 1,MPI_REAL8, sendto,0, comm, &
!!!! your code here !!!!
       ierr)
  call MPI_Irecv(rightdata, 1,MPI_REAL8, recvfrom,0, comm, &
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

  if (procno==0) then
     check = 2*procno+1
  else if (procno==nprocs-1) then
     check = 2*procno-1
  else
     check = 3*procno
  end if

  error = nprocs;
  if ( .not. ISAPPROX(mydata,check) ) then
     print *,"Data on process",procno,"should be",check," not",mydata
     error = procno
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
  
contains
  logical function ISAPPROX(x,y)
    real*8,intent(in) :: x,y
    isapprox = ( x==0. .and. abs(y)<1.e-14 ) &
         .or. ( y==0. .and. abs(x)<1.e-14 ) &
         .or. abs(x-y)/abs(x)<1.e-14
  end function ISAPPROX
  
end Program Isendirecv

