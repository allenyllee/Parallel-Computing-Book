! -*- f90 -*-
module Circle
  implicit none

  integer :: bound
  real :: step, ymin,ymax,ycur, xmin,xmax,xcur

  type,public :: coordinate
     real :: x,y
  end type coordinate

  integer :: blocksize,writepointer,readpointer
  type(coordinate),dimension(:),allocatable :: CoordinateBlock

contains

  subroutine GetArguments(steps,iters)
    implicit none
    integer,intent(out) :: steps,iters
    character(32) :: argtxt,dt
    integer :: i
    steps = 10; iters = 1000
    !call iarg(0,argtxt)
    do i=1,command_argument_count()-1 !iargc()
       call get_command_argument(i,argtxt) !call getarg(i,argtxt)
       if (argtxt(1:5)=='steps') then
          call get_command_argument(i+1,argtxt) !call getarg(i+1,argtxt)
          read(argtxt,*) steps
       end if
       if (argtxt(1:5)=='iters') then
          call get_command_argument(i+1,argtxt) !call getarg(i+1,argtxt)
          read(argtxt,*) iters
       end if
    end do
    return
  end subroutine GetArguments

  subroutine SetParameters(bs,steps_v,bound_v)
    implicit none
    integer,intent(in) :: bs,steps_v,bound_v

    step = 2./steps_v ; bound = bound_v
    ymin = -2.; ymax = +2.; ycur = ymin
    xmax = sqrt(4-ycur*ycur); xmin = -xmax; xcur = xmin

    blocksize = bs; writepointer = 1; readpointer = bs+1
    allocate(CoordinateBlock(bs))
  end subroutine SetParameters
  
  type(coordinate) function InvalidCoordinate()
    implicit none
    InvalidCoordinate%x = -5.; InvalidCoordinate%y = -5.
  end function InvalidCoordinate
  
  type(coordinate) function NextCoordinateInternal()
    implicit none
    if (xcur<xmax-step) then
       NextCoordinateInternal%x = xcur; NextCoordinateInternal%y = ycur; xcur = xcur+step
    else if (ycur<ymax-step) then
       ycur = ycur+step; xmax = sqrt(4-ycur*ycur); xmin = -xmax; xcur = xmin
       NextCoordinateInternal%x = xcur; NextCoordinateInternal%y = ycur; xcur = xcur+step
    else
       NextCoordinateInternal = InvalidCoordinate()
    end if
  end function NextCoordinateInternal

  ! Generate blocks of coordinates. The main reason for 
  ! introducing this level is so that in a block
  ! all coordinates are valid or all are invalid.
  type(coordinate) function NextCoordinate()
    implicit none
    type(coordinate) :: xy
    integer :: prev
    ! 1. writepointer>blocksize:
    !    get the next coordinate from the block
    !    increase readpointer
    !    if readpointer>blocksize:
    !      set writepointer to 1
    ! 2. writepointer==1:
    !    insert into block, increase writepointer
    !    exceptional case of invalid coordinate:
    !      fill the whole block with invalid, and 
    !      set writepointer beyond blocksize
    !    if writepointer>blocksize:
    !      exit loop and set readpointer to 1
    do
       if (writepointer.gt.blocksize) exit
       xy = NextCoordinateInternal()
       if (.not.IsValidCoordinate(xy)) then
          ! Once one invalid coorddinate has been generated, we
          ! fill the whole block with invalids */
          do prev=1,blocksize
             CoordinateBlock(prev) = xy
          end do
          ! and force read of the coordinate block
          writepointer = blocksize+1
       else
          ! default case: add coordinate to block
          CoordinateBlock(writepointer) = xy
          writepointer = writepointer+1
       end if
       ! if the block is full, set the read pointer at the beginning
       if (writepointer.gt.blocksize) then
          readpointer = 1
       end if
    end do
    NextCoordinate = CoordinateBlock(readpointer)
    readpointer = readpointer+1
    if (readpointer.gt.blocksize) then
       writepointer = 1
    end if
  end function NextCoordinate
  
  Logical function IsValidCoordinate(xy)
    type(coordinate),intent(in) :: xy
    if ( (xy%x<-3.) .and. (xy%y<-3.) ) then
       IsValidCoordinate = .FALSE.
    else
       IsValidCoordinate = .TRUE.
    end if
  end function IsValidCoordinate
  
  integer function Belongs(xy)
    type(coordinate),intent(in) :: xy
    real x,y,xx,yy
    integer :: it

    x = xy%x ; y = xy%y
    Belongs = 0
    do it=1,bound
       xx = x*x - y*y + xy%x
       yy = 2*x*y + xy%y
       x = xx; y = yy
       if (x*x+y*y>4.) then
          Belongs = it; exit
       end if
    end do
  end function Belongs

end module Circle

module Queue
  use Circle
  use mpi
  implicit none

  integer :: comm,ntids,mytid
! common bookkeeping for all queue variants
  integer :: TotalTasks
  double precision :: t_start,t_stop

contains

  ! all the other SomethingQueue modules need to call this
  subroutine QueueInit0(commv)
    implicit none
    integer,intent(in) :: commv
    integer :: ierr
    comm = commv
    call MPI_Comm_size(comm,ntids,ierr)
    call MPI_Comm_rank(comm,mytid,ierr)
    if (ntids<2) then
       write(*,*) "You need at least two processors"
       call MPI_Abort(comm,1,ierr)
    end if
    t_start = MPI_Wtime()
  end subroutine QueueInit0

  subroutine WaitForWork()
    use Circle
    implicit none
    Logical stop
    type(coordinate) :: xy
    real :: xyv(2)
    integer :: status(MPI_STATUS_SIZE),res,ierr
    integer :: count

    stop = .False.
    count = 0
    do
       call MPI_Recv(xyv,2,MPI_REAL,ntids-1,0, comm,status,ierr)
       xy%x = xyv(1); xy%y = xyv(2)
       stop = .NOT. IsValidCoordinate(xy)
       if (stop) then
          res = 0
       else
          count = count+1
          res = Belongs(xy)
       end if
       call MPI_Send(res,1,MPI_INTEGER,ntids-1,0,comm,ierr)
       if (stop) then
          exit
       end if
    end do

  end subroutine WaitForWork
    
end module Queue

