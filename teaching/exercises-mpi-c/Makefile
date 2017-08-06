# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-7
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
    randomput countdown onsesidedbuild \
    blockwrite viewwrite scatterwrite \
    jordan

include ../course-support/Make.course
include ../course-support/Make.inc

CFLAGS = -O1 -std=c99
.SECONDEXPANSION:
${EXERCISES} : $$@.c
	${MPI_C_COMPILER} ${CFLAGS} -o $@ $^

####
#### Regression tests
####

include ../course-support/Make.mpi_regress
run_hello : hello
