module SerialQueue
  use Circle
  use Queue
  implicit none

  integer FreeProcessor,TotalTasks

contains
  
  subroutine QueueInit(commv)
    integer,intent(in) :: commv
    call QueueInit0(commv)
    FreeProcessor = 0; TotalTasks = 0
  end subroutine QueueInit

  subroutine AddTask(xy)
    type(Coordinate),intent(in) :: xy
    real,dimension(2) :: xyv
    integer contribution,status,ierr

    xyv(1) = xy%x; xyv(2) = xy%y
    call MPI_Send(xyv,2,MPI_REAL,FreeProcessor,0,comm,ierr)
    call MPI_Recv(contribution,1,MPI_INTEGER,FreeProcessor,0,comm,status,ierr)
    TotalTasks = TotalTasks+1
    FreeProcessor = mod(FreeProcessor+1,ntids-1)
    
  end subroutine AddTask

  subroutine Complete()
    type(Coordinate) :: xy
    integer p
    xy = InvalidCoordinate(); FreeProcessor = 0
    do p=1,ntids-1
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
  call QueueInit(comm)

  steps = 200; iters = 1000
  call SetParameters(steps,iters)

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
  else
     call WaitForWork()
  end if

  call MPI_Finalize(ierr)

end program MandelSerial
