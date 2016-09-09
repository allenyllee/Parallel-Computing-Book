!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%
!  %%%% This program file is part of the book and course
!  %%%% "Parallel Computing"
!  %%%% by Victor Eijkhout, copyright 2013-5
!  %%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


  module allmod

  contains
    function f(x)
      real*8 :: x,f
      f = x*1.57
      return
    end function f
    
  end module allmod

Program Init

  use allmod

#include "mpif.h"

  integer :: nlocalpoints,ntotal_points,my_global_start
  real*8 :: stepsize
  real*8,dimension(:),allocatable :: array

#include "globalinit.F90"

!pragma local-f
  nlocalpoints = 10
  ntotal_points = ntids*nlocalpoints
  my_global_start = mytid*nlocalpoints;
  stepsize = 1./(ntotal_points-1);
  allocate(array(nlocalpoints))

  do i=1,nlocalpoint
     array(i) = f( (i+my_global_start)*stepsize )
  end do
!pragma end

  deallocate(array)
  call MPI_Finalize(err)


end program Init
