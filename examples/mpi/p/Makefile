################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-9
####
#### This is the makefile for the python examples
####     in examples/mpi/p
####
################################################################
################################################################

info ::

info ::
	@echo "make regression"
TESTS = \
    allreduce.py anysource.py bcast.py buffertoolarge.py \
    gatherv.py getfence.py irecvnonblock.py irecv_source.py \
    hello.py irecvloop.py persist.py putfence.py \
    pingpong.py scan.py scipingpong.py \
    commdup.py commsplit.py
TESTSWRONG = buffertoosmall.py 
TESTSNOT = exscan.py
TESTS_IN_PROGRESS =  interleave.py
include ../../../teaching/course-support/Make.mpi_run
PYTHON = python3
regression :
	@for p in ${TESTS} ; do \
	    echo "Testing $$p" \
	    && make run PROGRAM=$${p} \
	    ; \
	done

info ::
	@echo "make listsources"
SOURCES = mpi.py hello.py pingpong.py scipingpong.py bcast.py buffertoosmall.py buffertoolarge.py \
  gatherv.py allreduce.py getfence.py
listsources :
	@echo ${SOURCES}

include ../../Make.inc
