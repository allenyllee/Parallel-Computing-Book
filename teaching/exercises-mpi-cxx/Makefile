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
    commrank prime randommax jordan sumsquares onenorm \
    pingpong sendrecv stridesend \
    rightsend bucketblock bucketpipenonblock \
    isendirecv isendirecvarray procgrid procgridnonblock \
    rightput randomput onesidedbuild countdown \
    blockwrite viewwrite scatterwrite cubegather \
    ibarrierupdate rightgraph shareddata
info ::
	@echo "    [ OPTS=... ] (for instance: OPTS=\"-DN 100\")"
	@echo "    support options:"
	@echo "      bucketblock : N"

include ../course-support/Make.compile

.SECONDEXPANSION:
OPTS =
${EXERCISES} : $$@.o
	${CXX_LINKER} -o $@ $^

####
#### Regression tests
####

include ../course-support/Make.mpi_run
include ../course-support/Make.mpi_regress

include ../course-support/Make.clean
