# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-8
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
    commrank prime randommax pingpong sendrecv \
    isendirecv isendirecvarray sumsquares \
    stridesend cubegather procgrid \
    randomput countdown onesidedbuild \
    blockwrite viewwrite scatterwrite \
    jordan rightsend rightput rightgraph

.SECONDEXPANSION:
${EXERCISES} : $$@.c
	${MPI_C_COMPILER} ${CFLAGS} -o $@ $^

include ../course-support/Make.course
include ../course-support/Make.simgrid
ifdef TACC_TAU_DIR
include ../course-support/Make.tau
endif
include ../course-support/Make.inc

####
#### Regression tests
####

include ../course-support/Make.mpi_regress
