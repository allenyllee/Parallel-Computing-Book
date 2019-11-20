# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2017
#### eijkhout@tacc.utexas.edu
####
#### Makefile for short course OpenMP templates in C
####
################################################################

info ::

COURSE = omp
MODE = omp

info ::
	@echo "make EXERCISE (where EXERCISE=${EXERCISES})"
EXERCISES = parallel pi piadapt taskfactor

include ../../Make.clean

.SECONDEXPANSION:
${EXERCISES} : $$@.c
	${C_COMPILER} ${CFLAGS} -o $@ $^ -lm

####
#### Regression tests
####

include ../course-support/Make.omp_regress
