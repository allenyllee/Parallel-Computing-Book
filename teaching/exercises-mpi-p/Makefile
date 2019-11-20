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
    hello commrank prime randommax pingpong \
    rightsend isendirecv ibarrierupdate \
    onenorm bucketblock \
    stridesend cubegather \
    randomput rightput countdown onesidedbuild \
    procgrid blockwrite viewwrite

info ::
	@echo "make run [PROGRAM=prog (default=${PROGRAM})]"
	@echo "         [NP=nn (default=${NP})] [PYTHON=... (default=${PYTHON})]"
	@echo "         [OPTIONS=programoptions]"
.PHONY: run
NP = 4
PYTHON_clang = /opt/local/bin/python
PYTHON_gcc  = /opt/local/bin/python2.7
PYTHON_intel = python3
PYTHON = ${PYTHON_${TACC_FAMILY_COMPILER}}
PROGRAM = hello
OPTIONS =

##
## compilation is phony
##
.PHONY: ${EXERCISES}
${EXERCISES} :

PROGRAM_EXTENSION = .py

####
#### Regression tests
####
.PHONY: ${EXERCISES} 

include ../course-support/Make.mpi_regress
CATCHER = ../course-support/catchout -p ${PYTHON}

include ../../Make.clean
