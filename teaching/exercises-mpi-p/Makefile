# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### Victor Eijkhout
#### eijkhout@tacc.utexas.edu
####
#### Makefile for course exercises in Python
####
################################################################

info ::

COURSE = mpi
MODE = mpi
info ::
	@echo "nothing to make here...."
EXERCISES = \
    commrank prime pingpong \
    isendirecv \
    stridesend cubegather \
    randomput countdown \
    procgrid blockwrite viewwrite

include ../course-support/Make.course
include ../course-support/Make.inc

info ::
	@echo "make run [PROGRAM=prog (default=${PROGRAM})]"
	@echo "         [NP=nn (default=${NP})] [PYTHON=... (default=${PYTHON})]"
	@echo "         [OPTIONS=programoptions]"
.PHONY: run
NP = 4
PYTHON_gcc  = /opt/local/bin/python2.7
PYTHON_intel = python
PYTHON = ${PYTHON_${TACC_FAMILY_COMPILER}}
PROGRAM = hello
OPTIONS =
run :
	mpirun -np ${NP} ${PYTHON} ${PROGRAM}.py ${OPTIONS}

####
#### Regression tests
####
.PHONY: commrank prime pingpong \
    isendirecv \
    stridesend cubegather randomput \
    procgrid blockwrite viewwrite

include ../course-support/Make.mpi_regress
CATCHER = ../course-support/catchout -p ${PYTHON}

####
#### Cleaning
####

info ::
	@echo "make clean"
.PHONY: clean
clean ::
	@/bin/rm -rf *~ *.o *.pyc *.dSYM ddt.o* *.o[0-9]* *.dat

