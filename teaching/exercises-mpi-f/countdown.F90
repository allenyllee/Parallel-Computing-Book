! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-6
!****
!**** MPI Exercise 
!****
!****************************************************************/

Program CountDown
  use mpi
  implicit none
  
  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  !! data specifically for this program
  integer :: &
       counter_process,my_number, &
       err,step,is_zero,minus_one=-1
  integer :: the_window,window_data,window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: window_size,sizeofint,displacement=0
  real(4) :: randomfraction
  logical :: i_must_update

  call MPI_Init(ierr)

  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)


  !  // first set a unique random seed
  !  srand((int)(procno*(double)RAND_MAX/nprocs));
  
  counter_process = nprocs-1
  
  !!
  !! Create a window.
  !! We only need a nonzero size on the last process,
  !! which we label the `counter_process'
  !!
  call MPI_Sizeof(window_data,window_elt_size,ierr)
  if (procno==counter_process) then
     window_data = nprocs-1; window_size = 1
     call MPI_Win_create( &
          window_data,window_size,window_elt_size, &
          MPI_INFO_NULL,comm,the_window,err)
  else
     window_size = 0
     call MPI_Win_create( &
          window_data,window_size,window_elt_size, &
          MPI_INFO_NULL,comm,the_window,err)
  end if

  !!
  !! Loop forever:
  !! - at random times update the counter on the counter process
  !! - and read out the counter to see if we stop
  !!
  step = 0; is_zero = 0
  do 
     !! Some dynamic condition to determine whether we 
     !! update the global counter
     call random_number(randomfraction)
     i_must_update = randomfraction<.5/nprocs
     call MPI_Win_fence(0,the_window,err)
     if (i_must_update) then
	!!
        !! Exercise:
        !! - subtract one from the global counter
        !!   do you use MPI_Put or MPI_Accumulate?
        !!
        !!print *,"Proc",procno,"does acc in step",step
!!!! your code here !!!!
      end if
      !!
      !! Exercise:
      !! - Now read out the global counter to see if it's time to stop.
      !!   Here is the code:
      !!   MPI_Get
      !!   ( /* data on origin: */   is_zero, 1,MPI_INT, &
      !!     /* data on target: */   counter_process, displacement, 1,MPI_INT, &
      !!     the_window,err);
      !! - The problem is having having your data synchronized.
      !!   Where are you going to have Win_fence calls?
      !!   Is the one at the start of the loop enough?
      !!
!!!! your code here !!!!
      if (procno==counter_process) then
         print *,"Step:",step,"counter at ",window_data
      end if
      if (is_zero==0) exit
      step = step+1
   end do

   call MPI_Win_free(the_window,err)

   call MPI_Finalize(err)

 end Program CountDown
 
        
