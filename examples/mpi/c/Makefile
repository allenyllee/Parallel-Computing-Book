################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-9
####
#### This is the makefile for the C examples
####     in examples/mpi/c
####
################################################################
################################################################

MAKEFLAGS += --no-builtin-rules

info ::
	@echo "Do: make app (where app=${APPS})"

APPS = init thread env outerr abort \
    recvblock sendblock ssendblock sendrecv irecv_source irecvnonblock \
    probe ibarrier backintime reductpositive \
    putfence putfencealloc getfence anysource onesidedbuild onsidedmap \
    cancel exscan \
    postwaitwin passive \
    reduce reduceinplace allreduce allreduceinplace gather gatherv allgatherv \
    reducescatter trueextent \
    contiguous vector interleave indexed struct pack subarray \
    commdup_wrong commdup_right commsplit commsplittype intercomm \
    sharedshared \
    cart graph persist \
    spawn_manager spawn_worker \
    pingpong centraldiff \
    blockwrite scatterwrite
# not ready for prime time: mvp2d

listapps :
	@echo ${APPS}
listsources :
	@for a in ${APPS} ; do \
	  if [ -e $$a.c ] ; then echo $$a.c ; fi ; \
	  if [ -e $$a.cxx ] ; then echo $$a.cxx ; fi ; \
	done | awk 'BEGIN { v="" } {v = v " " $$0} END {print v}'
	@echo tools.{c,h}

info ::
	@echo "    [ MPICC=... (default: ${MPICC}) ]"
	@echo "    [ MPICXX=... (default: ${MPICXX}) ]"
MPICC = mpicc
MPICXX = mpicxx
ifdef TACC_TAU_DIR
  CC = tau_cc.sh
  CXX = tau_cxx.sh
else
  CC  = ${MPICC}
  CXX = ${MPICXX}
endif

OPTLEVEL=2
OPTFLAGS = -O${OPTLEVEL} -g
CFLAGS = -std=c99
CXXFLAGS = -std=c++17
EXTRAFLAGS = 
info ::
	@echo "    [ CORESPERNODE=nnn (default: ${CORESPERNODE})]"
	@echo "    [ EXTRAFLAGS=whatever]"
	@echo "      supported: EXTRAFLAGS=-DDEBUG"
CORESPERNODE = 16
%.o : %.c
	${MPICC}  ${CFLAGS} ${OPTFLAGS} ${EXTRAFLAGS} -DCORES_PER_NODE=${CORESPERNODE} \
	  -c $*.c
%.o : %.cxx
	${MPICXX} ${CXXFLAGS} ${OPTFLAGS} ${EXTRAFLAGS} -DCORES_PER_NODE=${CORESPERNODE} \
	  -c $*.cxx

app_dependencies  = tools.o
.SECONDEXPANSION:
${APPS} : $$@.o
	${MPICC} -o $@ $@.o
${patsubst %,%.o,${APPS}} tools.o : tools.h globalinit.c

# rules for submitting to the queue and doing tau analysis
info ::
	@echo
	@echo "make submit EXECUTABLE=<any prog> OPTIONS=<whatever>"
	@echo "     do qsub jobscript, where the jobscript does"
	@echo "     ibrun EXECUTABLE OPTIONS"
	@echo "     with the tau environment variables set to catch the trace"
	@echo "     files in a directory named tautrace_EXECUTABLE"
EXECUTABLE = init
OPTIONS = 
submit :
	@export TAU_EXT=`if [ ! -z "${EXECUTABLE}" ] ; then echo "_" ; fi`${EXECUTABLE} ; \
	export TAU_DUMP_DIR=`pwd`/tautrace$$TAU_EXT ; \
	  echo "tau output to: <$$TAU_DUMP_DIR>" ; \
	  rm -rf $${TAU_DUMP_DIR}; mkdir -p $${TAU_DUMP_DIR} ; \
	  TAU_TRACE=1 TAU_PROFILE=1 \
	  TRACEDIR=$${TAU_DUMP_DIR} \
	  PROFILEDIR=$${TAU_DUMP_DIR} \
	    EXECUTABLE=${EXECUTABLE} OPTIONS="${OPTIONS}" \
	      qsub jobscript
info ::
	@echo "make idevrun EXECUTABLE=<any prog> [OPTIONS=<whatever>]"
	@echo "     do ibrun EXECUTABLE OPTIONS"
	@echo "     with the tau environment variables set to catch the trace"
	@echo "     files in a directory named tautrace_EXECUTABLE"
idevrun :
	@if [ -z "${EXECUTABLE}" ] ; then \
	   echo "Usage: make idevrun EXECUTABLE=... [OPTIONS=...]"; exit 1 ; fi
	@export TAU_EXT="_${EXECUTABLE}" ; \
	export TAU_DUMP_DIR=`pwd`/tautrace$$TAU_EXT ; \
	  rm -rf $${TAU_DUMP_DIR}; mkdir -p $${TAU_DUMP_DIR} ; \
	  TAU_TRACE=1 TAU_PROFILE=1 \
	  TRACEDIR=$${TAU_DUMP_DIR} \
	  PROFILEDIR=$${TAU_DUMP_DIR} \
	    ibrun ${EXECUTABLE} ${OPTIONS}
info ::
	@echo "make tau EXECUTABLE=..."
	@echo "     do a tau postprocessing of running EXECUTABLE;"
	@echo "     this leaves a file taulog_EXECUTABLE.slog2"
tau :
	@export HOME=`pwd` ; \
	export TAU_EXT=`if [ ! -z "${EXECUTABLE}" ] ; then echo "_" ; fi`${EXECUTABLE} ; \
	export TAU_DUMP_DIR=`pwd`/tautrace$$TAU_EXT ; \
	  cd $$TAU_DUMP_DIR ; \
	  echo ".. analyzing files in <$$TAU_DUMP_DIR>" ; \
	  rm -f tau.trc tau.edf ; \
	  tau_treemerge.pl ; \
	  tau2slog2 tau.trc tau.edf -o $$HOME/taulog$$TAU_EXT.slog2

info ::
	@echo "make regression : do regression test on all apps"
init_args = 5
noap = `make listapps`
regression : total_clean
	@for app in `make listapps` ; do \
	  make $$app >/dev/null 2>&1 ; \
	  if [ ! -e $$app ] ; then \
	    echo "Application <<$$app>> failed to build" ; \
	  fi ; \
	done ; echo "Finished regression building"
aprun :
	  else \
	    eval appargs=\"\\\$${$${app}_args}\" ; \
	    echo $$appargs ; \
	    if [ ! -z "$$appargs" ] ; then \
	      eval mpirun -np 4 $$app $$appargs ; \
	    fi ; \
	foo
info ::
	@echo "make regress_run PROGRAM=..."
PROGRAM = onesidedbuild
NP = 4
regress_run :
	@mpirun -np ${NP} ./catchout ${PROGRAM}
	@for c in `seq 0 $$(( ${NP} - 1 ))` ; do \
	  echo "==== p: $$c ====" ; \
	  cat ${PROGRAM}.out$$c ; \
	done

noregression :
	-@export haserror=0 ; \
	for p in ${APPS} ; do \
	  make $$p >$$p.err 2>&1 ; \
	  if [ ! -f $$p ] ; then \
	    if [ $$haserror == "0" ] ; then export haserror=1 ; echo ; fi ; \
	    echo "Regression failed compilation of $$p" ; \
	    cat $$p.err ; echo ; \
	  fi ; \
	  rm -f $$p.err ; \
	done ; \
	if [ $$haserror == "0" ] ; then \
	  echo "Regression succesfully concluded" ; \
	fi

.PHONY: run_backintime
info ::
	@echo "make run_backintime"
run_backintime : backintime
	for i in `seq 1 150` ; do \
	  mpirun -np 3 ./backintime ; \
	done | grep back

.PHONY: run_abort
run_abort : abort
	-@${MPIRUN} -np ${NP} ./abort ; \
	  echo "Return code from ${MPIRUN} is <<$$?>>"

include ../../Make.inc
clean ::
	@rm -f *.dat *.out[0-9]*
