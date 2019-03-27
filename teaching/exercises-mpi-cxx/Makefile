# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-9
#### eijkhout@tacc.utexas.edu
####
#### Makefile for MPI course exercises in C++
####
################################################################

info ::

COURSE = mpi
MODE = mpi

info ::
	@echo "make EXERCISE (where EXERCISE=${EXERCISES})"
EXERCISES = hello \
    commrank prime randommax jordan pingpong sendrecv \
    rightsend bucketblock stridesend \
    isendirecv isendirecvarray procgrid randomput onesidedbuild \
    blockwrite scatterwrite cubegather \
    ibarrierupdate

include ../course-support/Make.template
include ../course-support/Make.course
include ../course-support/Make.inc

.SECONDEXPANSION:
${EXERCISES} : $$@.cxx
	${MPI_CXX_COMPILER} ${CXXFLAGS} -o $@ $^

clean ::
	@/bin/rm -f *.out[0-9]*

####
#### Regression tests
####

include ../course-support/Make.mpi_regress
