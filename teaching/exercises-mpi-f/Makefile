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
    cubegather procgrid \
    rightput randomput countdown onesidedbuild \
    blockwrite
OTHERPROGRAMS = write

FFLAGS = -O1
.SECONDEXPANSION:
${EXERCISES} : $$@.F90
	${MPI_F_COMPILER} ${FFLAGS} -o $@ $^

####
#### Regression tests
####

include ../course-support/Make.inc
include ../course-support/Make.mpi_regress
run_hello : hello
