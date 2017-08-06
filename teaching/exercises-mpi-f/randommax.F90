! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-7
!****
!**** MPI Exercise for MPI_Reduce
!****
!****************************************************************/

Program RandomMax

  use mpi
  implicit none

  integer :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr, i

  ! stuff for the random number generator
  integer :: randomint,sender
  integer :: randsize
  integer,allocatable,dimension(:) :: randseed
  real :: my_random,global_random, scaled_random,sum_random

  call MPI_Init(ierr); 
  comm = MPI_COMM_WORLD;
  call MPI_Comm_rank(comm,procno,ierr); 
  call MPI_Comm_size(comm,nprocs,ierr); 

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
  print *,"Process",procno,"has random value",my_random

  !!
  !! Exercise part 1:
  !! - compute the maximum random value on process zero
  !!
  call MPI_Reduce( &
!!!! your code here !!!!
       comm,ierr)
  if (procno==0) then
     print *,"The maximum number is",global_random
  end if
  
  !!
  !! Exercise part 2:
  !! (re-enable this part of the code by removing #if and #endif lines)
  !! -- compute the sum of the values, everywhere
  !! -- scale your number by that sum
  !! -- check that the sum of scaled values is 1
  !!
#if 0
  call MPI_Allreduce( &
!!!! your code here !!!!
!!!! your code here !!!!
       comm,ierr)
  if (abs(sum_random-1.)>1.e-14) then
     print *,"Suspicious sum",sum_random,"on process",procno
  end if
#endif
  
  if (procno==0) then
     print *,"Success: all tests pass"
  end if
  
  call MPI_Finalize(ierr)
  
end Program RandomMax

