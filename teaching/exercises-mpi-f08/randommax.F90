! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for MPI_Reduce
!**** fortran 2008 version
!****
!****************************************************************/

Program RandomMax

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,error,errors, ierr,i

  ! stuff for the random number generator
  integer :: randomint,sender
  integer :: randsize
  integer,allocatable,dimension(:) :: randseed
  real :: my_random,scaled_random,sum_random,sum_scaled_random

  call MPI_Init(); 
  call MPI_Comm_rank(comm,procno); 
  call MPI_Comm_size(comm,nprocs); 

  !!
  !! Initialize the random number generator
  !! using the process number to get a unique seed
  !!
  call random_seed(size=randsize)
  allocate(randseed(randsize))
  do i=1,randsize
     randseed(i) = 1023*procno
  end do
  call random_seed(put=randseed)

  !! My own random
  call random_number(my_random)
  print '(("Process",i4,x,"has random value",x,f9.7))', procno,my_random

  !!
  !! Exercise:
  !! -- compute the sum of the values, everywhere
  !! -- scale your number by that sum
  !! -- check that the sum of scaled values is 1
  !!
  call MPI_Allreduce( &
!!!! your code here !!!!
       comm)
  scaled_random = my_random / sum_random
  call MPI_Allreduce( &
!!!! your code here !!!!
       comm)

  !!
  !! Correctness test
  !!
  if (abs(sum_scaled_random-1.)>1.e-5) then
     print *,"Suspicious sum",sum_scaled_random,"on rank",procno
     error = procno
  end if
  call MPI_Allreduce(error,errors,1,MPI_INTEGER,MPI_MIN,comm)
  if (procno==0) then
     if (errors==nprocs) then
        print *,"Part 1 finished; all results correct"
     else
        print *,"Part 1: first error occurred on rank",errors
     end if
  end if

  if (procno==0) then
     print *,"Success: all tests pass"
  end if
  
  call MPI_Finalize()
  
end Program RandomMax

