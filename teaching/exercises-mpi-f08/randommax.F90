! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-8
!****
!**** MPI Exercise for MPI_Reduce
!**** fortran 2008 version
!****
!****************************************************************/

Program RandomMax

  use mpi_f08
  implicit none

  type(MPI_Comm) :: comm = MPI_COMM_WORLD
  integer :: nprocs, procno,ierr, i

  ! stuff for the random number generator
  integer :: randomint,sender
  integer :: randsize
  integer,allocatable,dimension(:) :: randseed
  real :: my_random,global_random, scaled_random,sum_random,sum_scaled_random

  call MPI_Init(ierr); 
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
  !! (re-enable this part of the code by removing #if and #endif lines)
  !! -- compute the sum of the values, everywhere
  !! -- scale your number by that sum
  !! -- check that the sum of scaled values is 1
  !!
  call MPI_Allreduce( &
!!!! your code here !!!!
       comm,ierr)
  scaled_random = my_random / sum_random
  call MPI_Allreduce( &
!!!! your code here !!!!
       comm,ierr)
  if (abs(sum_scaled_random-1.)>1.e-5) then
     print *,"Suspicious sum",sum_scaled_random,"on process",procno
  end if
  
#if 0
  !!
  !! Exercise part 2:
  !! - compute the maximum random value on process zero
  !!
  call MPI_Reduce( &
!!!! your code here !!!!
       comm,ierr)
  if (procno==0) then
     print *,"The maximum number is",global_random
  end if
#endif
  
  if (procno==0) then
     print *,"Success: all tests pass"
  end if
  
  call MPI_Finalize(ierr)
  
end Program RandomMax

