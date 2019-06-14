# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-9
#### eijkhout@tacc.utexas.edu
####
#### Makefile for  MPI course exercises in Fortran2008
####
################################################################

info ::

COURSE = mpi
MODE = mpi
info ::
	@echo "make EXERCISE (where EXERCISE=${CEXERCISES} ${FEXERCISES})"
EXERCISES = hello \
    commrank prime randommax jordan \
    pingpong sendrecv sumsquares \
    rightsend rightput rightgraph \
    bucketblock bucketpipenonblock \
    isendirecv isendirecvarray status \
    stridesend cubegather procgrid \
    randomput onesidedbuild countdown \
    blockwrite ibarrierupdate
OTHERPROGRAMS = write

include ../course-support/Make.inc

FFLAGS = -O1
.SECONDEXPANSION:
${EXERCISES} : $$@.F90
	${MPI_F_COMPILER} ${FFLAGS} -o $@ $^

####
#### Regression tests
####

include ../course-support/Make.mpi_regress
run_hello : hello
