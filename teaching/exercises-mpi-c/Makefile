# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-9
#### eijkhout@tacc.utexas.edu
####
#### Makefile for MPI course exercises in C
####
################################################################

info ::

COURSE = mpi
MODE = mpi

info ::
	@echo "make EXERCISE (where EXERCISE=${EXERCISES})"
EXERCISES = hello \
    commrank prime randommax onenorm \
    pingpong sendrecv \
    isendirecv isendirecvarray sumsquares \
    bucketblock bucketnonblock bucketpipe bucketpipenonblock \
    stridesend cubegather procgrid procgridnonblock \
    randomput countdown onesidedbuild \
    blockwrite viewwrite scatterwrite \
    jordan rightsend rightput rightgraph \
    ibarrierupdate shareddata
info ::
	@echo "    [ OPTS=... ] (for instance: OPTS=\"-DN 100\")"
	@echo "    support options:"
	@echo "      bucketblock : N"

.SECONDEXPANSION:
OPTS = 
${EXERCISES} : $$@.c
	${MPI_C_COMPILER} \
	  ${CFLAGS} ${OPTS} \
	  -o $@ $^

include ../course-support/Make.simgrid
ifdef TACC_TAU_DIR
include ../course-support/Make.tau
endif
include ../course-support/Make.inc

####
#### Regression tests
####

include ../course-support/Make.mpi_regress
