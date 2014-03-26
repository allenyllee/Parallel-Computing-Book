! -*- f90 -*-
module SerialQueue
  use Circle
  use Queue

  implicit none

  integer FreeProcessor,TotalTasks

contains
  
  subroutine QueueInit(commv)
    use Queue
    integer,intent(in) :: commv
    call QueueInit0(commv)
    FreeProcessor = 0; TotalTasks = 0
  end subroutine QueueInit

  subroutine AddTask(xy)
    use Queue
    type(Coordinate),intent(in) :: xy
    real :: xyv(2)
    integer contribution,status(MPI_STATUS_SIZE),ierr

    xyv(1) = xy%x; xyv(2) = xy%y
    call MPI_Send(xyv,2,MPI_REAL,FreeProcessor,0,comm,ierr)
    if (IsValidCoordinate(xy)) then
       call MPI_Recv(contribution,1,MPI_INTEGER,FreeProcessor,0,comm,status,ierr)
       !write(*,*) "coord",xy%x,xy%y,"on proc",FreeProcessor,"gives",contribution
!       call coordinate_to_image(xy,contribution)
       TotalTasks = TotalTasks+1
    else
       print *,"terminating",FreeProcessor
    end if
    FreeProcessor = mod(FreeProcessor+1,ntids-1)
    
  end subroutine AddTask

  subroutine Complete()
    use Queue
    type(Coordinate) :: xy
    integer p
    xy = InvalidCoordinate(); FreeProcessor = 0
    do p=0,ntids-1
       call AddTask(xy)
    end do
  end subroutine Complete

end module SerialQueue

program MandelSerial
  use Circle
  use SerialQueue
  implicit none
  type(Coordinate) :: xy
  integer ierr
  integer steps,iters

  call MPI_Init(ierr)
  comm = MPI_COMM_WORLD
  call MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN,ierr)
  call QueueInit(comm)

!  steps = 10; iters = 1000
  call GetArguments(steps,iters)
  call SetParameters(ntids,steps,iters)

  if (mytid.eq.ntids-1) then
     do
        xy = NextCoordinate()
        if (IsValidCoordinate(xy)) then
           call AddTask(xy)
        else
           exit
        end if
     end do
     call Complete()
     print *,"Performed",TotalTasks,"tasks"
  else
     call WaitForWork()
  end if

  call MPI_Finalize(ierr)

end program MandelSerial
