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
    real :: xyv(2)
    integer contribution,status,ierr

    xyv(1) = xy%x; xyv(2) = xy%y
    write(*,*) "to",FreeProcessor
    call MPI_Send(xyv,2,MPI_REAL,FreeProcessor,0,comm,ierr)
    write(*,*) ierr,"from",FreeProcessor
    call MPI_Recv(contribution,1,MPI_INTEGER,FreeProcessor,0,comm,status,ierr)
    write(*,*) ierr,"done",FreeProcessor
    if (IsValidCoordinate(xy)) then
       TotalTasks = TotalTasks+1
    end if
    FreeProcessor = mod(FreeProcessor+1,ntids-1)
    
  end subroutine AddTask

  subroutine Complete()
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

  steps = 200; iters = 1000
  call SetParameters(steps,iters)

  if (mytid.eq.ntids-1) then
     do
        xy = NextCoordinate()
        write(*,*) "next",xy%x,xy%y
        if (IsValidCoordinate(xy)) then
           call AddTask(xy)
           write(*,*) "added"
        else
           write(*,*) "quit"
           exit
        end if
     end do
     write(*,*) "done"
     call Complete()
  else
     call WaitForWork()
  end if

  call MPI_Finalize(ierr)

end program MandelSerial
