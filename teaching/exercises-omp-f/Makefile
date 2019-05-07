# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### Victor Eijkhout
#### eijkhout@tacc.utexas.edu
####
#### Makefile for short course OpenMP templates in Fortran
####
################################################################

info ::

COURSE = omp
info ::
	@echo "make EXERCISE (where EXERCISE=${EXERCISES})"
EXERCISES = parallel pi piadapt taskfactor

include ../course-support/Make.inc
include ../course-support/Make.omp_regress

.SECONDEXPANSION:
${EXERCISES} : $$@.F90
	${F_COMPILER} ${FFLAGS} -o $@ $^
