! -*- f90 -*-
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


Program ExScan

  implicit none

#include "mpif.h"

  integer,dimension(:),allocatable :: recv_buffer
  integer :: status(MPI_STATUS_SIZE)
  real :: randomvalue
  integer :: randomint,sender

!pragma myfirst-f
  integer :: my_first=0,localsize
! localsize = .... result of local computation ...
!pragma end

#include "globalinit.F90"

  call random_number( randomvalue )
  localsize = 10*(randomvalue+1)

!pragma myfirst-f
  ! find myfirst local based on the local sizes
  call MPI_Exscan(localsize,my_first, &
       1,MPI_INTEGER,MPI_SUM,comm,err)
!pragma end

  write(6,'("[",i3,"] localsizes ",i3," first ",i3)') mytid,localsize,my_first

  call MPI_Finalize(err)


end program ExScan
