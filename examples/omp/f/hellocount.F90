!****************************************************************
!****
!**** hellocount.F90
!****
!****************************************************************
Program Hello

  use omp_lib
  integer :: nthreads,mythread
  
!pragma hello-who-omp-f
!$omp parallel
  nthreads = omp_get_num_threads()
  mythread = omp_get_thread_num()
  write(*,'("Hello from",i3," out of",i3)') mythread,nthreads
!$omp end parallel
!pragma end

end Program Hello
