module Circle
  implicit none

  integer :: step,bound;
  real :: ymin,ymax,y, xmin,xmax,x

  type coordinate
     real :: x,y
  end type coordinate

contains
  subroutine SetParameters(steps_v,bound_v)
    integer,intent(in) :: steps_v,bound_v
    step = 2./steps_v ; bound = bound_v
    
    ymin = -2.; ymax = +2.; y = ymin
    xmax = sqrt(4-y*y); xmin = -xmax; x = xmin
  end subroutine SetParameters
  
  type(coordinate) function InvalidCoordinate()
    InvalidCoordinate%x = -5.; InvalidCoordinate%y = -5.
  end function InvalidCoordinate
  
  type(coordinate) function NextCoordinate()
    if (x<xmax-step) then
       NextCoordinate%x = x; NextCoordinate%y = y; x = x+step
    else if (y<ymax-step) then
       y = y+step; xmax = sqrt(4-y*y); xmin = -xmax; x = xmin
       NextCoordinate%x = x; NextCoordinate%y = y; x = x+step
    else
       NextCoordinate = InvalidCoordinate()
    end if
    write(*,*) "coord",NextCoordinate%x,NextCoordinate%y
  end function NextCoordinate
  
  Logical function IsValidCoordinate(xy)
    type(Coordinate),intent(in) :: xy
    if (xy%x>-3. .and. xy%y>-3.) then
       IsValidCoordinate = .TRUE.
    else
       IsValidCoordinate = .FALSE.
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
  use mpi
  implicit none

  integer :: comm,ntids,mytid

contains

  subroutine QueueInit0(commv)
    integer,intent(in) :: commv
    integer :: ierr
    comm = commv
    call MPI_Comm_size(comm,ntids,ierr)
    call MPI_Comm_rank(comm,mytid,ierr)
    if (ntids<2) then
       write(*,*) "You need at least two processors"
       call MPI_Abort(comm,1,ierr)
    end if
  end subroutine QueueInit0

  subroutine WaitForWork()
    use Circle
    Logical stop
    type(Coordinate) :: xy
    real :: xyv(2)
    integer :: status,ntids,res,ierr

    stop = .False.
    do
!       write(*,*) mytid,"recving"
       call MPI_Recv(xyv,2,MPI_REAL,ntids-1,0, comm,status,ierr)
       xy%x = xyv(1); xy%y = xyv(2)
       stop = .NOT. IsValidCoordinate(xy)
       if (stop) then
          res = 0
       else
          res = Belongs(xy)
!          write(*,*) "proc",mytid,"on",x,xy%x,xy%y,res
       end if
       call MPI_Send(res,1,MPI_INTEGER,ntids-1,0,comm,status,ierr)
       if (stop) then
          exit
       end if
    end do

  end subroutine WaitForWork
    
end module Queue

