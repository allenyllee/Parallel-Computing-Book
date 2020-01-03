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
APPS = \
    allreduce anysource bcast buffertoolarge \
    gatherv getfence irecvnonblock irecv_source \
    hello irecvloop persist putfence \
    pingpong scan scipingpong \
    commdup commsplit spawn_manager spawn_worker

info ::
	@echo "make listapps : list all application targets"
.PHONY: listapps
listapps :
	@echo ${APPS}
	@for a in ${APPS} ; do \
	    if [ ! -f $${a}.py ] ; then \
	        echo "Warning: no source file for <<$${a}>>" \
	        ; \
	    fi \
	    ; \
	done

info ::
	@echo "make run_spawn : process spawning example"
.PHONY: run_spawn
run_spawn :
	@make regression APPS=spawn_manager

TESTSWRONG = buffertoosmall.py 
TESTSNOT = exscan.py
TESTS_IN_PROGRESS =  interleave.py
include ../../../teaching/course-support/Make.mpi_run
PYTHON = python3
regression :
	@for p in ${APPS} ; do \
	    echo "Testing $$p" \
	    && if [ $${p} = "spawn_manager" ] ; then \
	        export ENVOPTIONS="MY_MPIRUN_OPTIONS=\"-usize 8\"" ; \
	       fi \
	    && make run PROGRAM=$${p}.py ENVOPTIONS="$${ENVOPTIONS}" \
	    ; \
	done

info ::
	@echo "make listsources"
SOURCES = mpi.py hello.py pingpong.py scipingpong.py bcast.py buffertoosmall.py buffertoolarge.py \
  gatherv.py allreduce.py getfence.py
listsources :
	@echo ${SOURCES}

include  ../../../makefiles/Make.clean
