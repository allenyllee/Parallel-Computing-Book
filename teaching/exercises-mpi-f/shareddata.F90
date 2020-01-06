! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming with MPI and OpenMP'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-9
!****
!**** MPI Exercise
!**** shareddata.F90 : MPI shared memory
!****
!****************************************************************/

Program SharedData
  use mpi
  implicit none

  integer :: nprocs, procno,ierr
  integer :: comm = MPI_COMM_WORLD

  ! communicators
  type(MPI_Comm) :: nodecomm, crosscomm
  integer :: onnode_procno, nodeid

  ! window stuff
  integer :: the_window,window_elt_size
  integer(kind=MPI_ADDRESS_KIND) :: window_size,sizeofint,displacement=0
#define MAXFULL 100
  integer :: mytable(MAXFULL);

  ! other data for this exercise
  real*8 :: shared_data

  call MPI_Init(ierr)
  call MPI_Comm_size(comm,nprocs,ierr)
  call MPI_Comm_rank(comm,procno,ierr)

  !!
  !! Exercise:
  !! -- construct `nodecomm': the communicator that contains all processes on this node
  !!    the correct type is in the book/slides
  !! Find the subcommunicator on the node
  !! and get the procno on the node
  !!
  call MPI_Comm_split_type(comm, &
!!!! your code here !!!!
       procno, MPI_INFO_NULL, nodecomm)

  !!
  !! Exercise:
  !! -- compute the rank on the node
  call MPI_Comm_rank( &
!!!! your code here !!!!
       onnode_procno)

  !!
  !! Exercise:
  !! -- make subcommunicators with one rank per node each
  !!    the procno in that communicator should be the node ID
  !!
  call MPI_Comm_split( &
!!!! your code here !!!!
       procno,crosscomm)
  call MPI_Comm_rank(crosscomm,nodeid);
  print '("[",i3,"] has node,onnode coordinate ",i3,",",i3)',&
       procno,nodeid,onnode_procno

  !!
  !! Create data on global process zero and broadcast it to process zero on other nodes
  !!
#define REFVAL 3.14
  shared_data = REFVAL
  if (onnode_procno==0) then
     call MPI_Bcast(shared_data,1,MPI_REAL8,0,crosscomm)
     print '("[",i3,"]" Head nodes now have shared data: ",f9.5)',procno,shared_data
  end if
  
    
  mytable(1) = 0; ! set stack pointer to first open location
  call MPI_Sizeof(mytable(1),window_elt_size,ierr)
  window_size = MAXFULL*window_elt_size
  call MPI_Win_create( &
       mytable,window_size,window_elt_size, &
       MPI_INFO_NULL,comm,the_window,ierr)

  do inum=1,NUMBERS
     call random_number(randomfraction)
     other_process = nprocs * randomfraction
     targets(inum) = other_process

     !!
     !! - Lock the window on the other process
     !! - Get first open location on the target and increment this
     !! - Write the random number
     !!
     call MPI_Win_lock(MPI_LOCK_EXCLUSIVE,other_process,0,the_window,ierr)

     pointer_loc = 0
     !!
     !! Exercise:
     !! - add 1 to the stack pointer on the other process
     !! - which routine do you use? the window is locked so maybe put/get?
     !!
!!!! your code here !!!!

     !!
     !! Exercise: write the number
     !! - `stackpointer' now describes where you can safely write in the other process
     !! - what call are you using to transfer data? are there possible conflicts?
     !! - is further synchronization needed?
     !!
     number_for_other = procno
     offset = stack_pointer+1
!!!! your code here !!!!

     !! Unlock the window so that other processes can access it
     call MPI_Win_unlock(other_process,the_window,ierr)
  end do
  
  call MPI_Win_free(the_window,ierr)

  if (nprocs>99) print *,"Warning: no space for procno in format"
  if (NUMBERS>50) print *,"Warning: no space for targets in format"
  print '("[",i2"] wrote its id to:",50i2)',procno,(targets(i),i=1,NUMBERS)
  print '("[",i2"] received:",50i2)',procno,(mytable(i),i=1,mytable(1))
  
  call MPI_Finalize(ierr)
  
end Program SharedData

