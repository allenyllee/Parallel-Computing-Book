Program Hello

!pragma hello-omp-f
!$omp parallel
  print *,"Hello world!"
!$omp end parallel
!pragma end

    ! int mythread,nthreads;
    ! nthreads = omp_get_num_threads();
    ! mythread = omp_get_thread_num();
    ! printf("Hello from %d out of %d\n",mythread,nthreads);

end Program Hello
