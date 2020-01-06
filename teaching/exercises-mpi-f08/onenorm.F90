! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise for user-defined operators
!**** fortran 2008 version
!****
!****************************************************************/

Program OneNorm

  use mpi_f08
  implicit none

  integer :: nprocs, procno,ierr
  type(MPI_Comm) :: comm

  !! data specific for this program
  Type(MPI_Op) :: calc_one
  integer :: nlocal,nglobal,i,itmp
  real :: actual_norm
  real :: data, data_one_norm
  !real,pointer :: data, data_one_norm
  !allocate(data,data_one_norm)

  call MPI_Init()
  comm = MPI_COMM_WORLD
  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  data = -1.e0

  call MPI_Op_create(one_norm,.true.,calc_one)
  ! https://stackoverflow.com/tags/fortran-iso-c-binding/info
  call MPI_Allreduce(data,data_one_norm,1,MPI_REAL,calc_one,comm)
  actual_norm = nprocs
  if (data_one_norm/=actual_norm) then
     print *,procno,": result",data_one_norm," should be",actual_norm
  else if (procno==0) then
     print *,"Success"
  end if

  call MPI_Finalize()
  
contains
  subroutine one_norm(invec,inoutvec,len,datatype) ! procedure(MPI_User_function)
    use, intrinsic :: iso_c_binding, only : c_ptr
    use mpi_f08_types, only : MPI_Datatype
    implicit none
    ! parameters
    type(c_ptr), value :: invec,inoutvec
    Integer :: len
    Type(MPI_Datatype) :: datatype

    ! local variables
    Real,pointer :: m,n,r

    ! extract c_ptr to f pointer
    call c_f_pointer(invec,n)
    call c_f_pointer(inoutvec,r)
    !n = invec; r = inoutvec
    !!
    !! Exercise:
    !! -- do the reduction step for a 1-norm
    !!
!!!! your code here !!!!
    call f_c_pointer(m,inoutvec)
    !inoutvec(1) = m
  end subroutine one_norm
end Program OneNorm
