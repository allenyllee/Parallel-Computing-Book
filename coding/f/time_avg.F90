Program AvgDuration

  use mpi

  integer :: mytid, comm, ierr
  double precision :: tstart,tstop,duration,x,sendbuf,random_value
  integer :: max_iter,i

  call MPI_Init(ierr)
  comm = MPI_COMM_WORLD
  call MPI_Comm_rank(comm,mytid,ierr)

! do something random, and measure how long it takes
  call random_number(random_value)
  max_iter = 1000000.*random_value
  x = 999.

  tstart = MPI_Wtime()
  do i=0,max_iter
     x = x+sin(x)
  end do
  tstop = MPI_Wtime()
  duration = tstop-tstart

! find the maximum time over all processors
  if (mytid.eq.0) then
      sendbuf = MPI_IN_PLACE
   else
      sendbuf = duration
   end if
   call MPI_Reduce(sendbuf,duration,1,MPI_DOUBLE_PRECISION,MPI_MAX,0,comm,ierr)
   if (mytid.eq.0) then
      print *,"Average duration:",duration,"; result=",x
   end if

  call MPI_Finalize(ierr)

end program AvgDuration
