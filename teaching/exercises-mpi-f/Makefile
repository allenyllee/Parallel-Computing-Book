# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-9
#### eijkhout@tacc.utexas.edu
####
#### Makefile for  MPI course exercises in Fortran
####
################################################################

info ::

COURSE = mpi
MODE = mpi
info ::
	@echo "make EXERCISE (where EXERCISE=${EXERCISES}"
EXERCISES = hello \
    commrank prime randommax jordan \
    pingpong sendrecv sumsquares \
    rightsend bucketblock \
    isendirecv isendirecvarray \
    cubegather onenorm procgrid \
    rightput randomput countdown onesidedbuild \
    blockwrite shareddata
OTHERPROGRAMS = write

include ../course-support/Make.compile

.SECONDEXPANSION:
${EXERCISES} : $$@.o
	${F_LINKER} -o $@ $^

####
#### Regression tests
####

include ../course-support/Make.mpi_run
include ../course-support/Make.mpi_regress
run_hello : hello

include ../../Make.clean
