################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013/4
####
#### This is the makefile for the examples/mpi/c
####
################################################################
################################################################

info ::
	@echo "Do: make app (where app=${APPS})"

APPS = hello init thread \
    recvblock sendblock ssendblock sendrecv irecv_source \
    putfence getfence randomput anysource cancel exscan \
    postwaitwin passive \
    reduce reduceinplace allreduce allreduceinplace gather gatherv \
    reducescatter \
    contiguous vector interleave indexed struct pack \
    commdup_wrong commdup_right cart persist \
    pingpong mvp2d centraldiff 

listapps :
	@echo ${APPS}
listsources :
	@for a in ${APPS} ; do \
	  if [ -e $$a.c ] ; then echo $$a.c ; fi ; \
	  if [ -e $$a.cxx ] ; then echo $$a.cxx ; fi ; \
	done | awk 'BEGIN { v="" } {v = v " " $$0} END {print v}'
	@echo tools.{c,h}

ifdef TACC_TAU_DIR
  CC = tau_cc.sh
  CXX = tau_cxx.sh
else
  CC  = mpicc -std=c99
  CXX = mpicxx
endif

DEBUG=OPT
OPT_flag = -O2 -g
DEBUG_flag = -O0 -g

info ::
	@echo "    debug flag: DEBUG=DEBUG/OPT (default: ${DEBUG})]"
%.o : %.c
	${CC} ${${DEBUG}_flag} -D${DEBUG} \
	  -c $*.c
%.o : %.cxx
	${CXX} ${${DEBUG}_flag} -D${DEBUG} \
	  -c $*.cxx

app_dependencies  = tools.o
.SECONDEXPANSION:
${APPS} : $$@.o ${app_dependencies}
	${CXX} -o $@ $@.o ${app_dependencies}
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
	@for app in init ; do \
	  make $$app >/dev/null 2>&1 ; \
	  if [ ! -e $$app ] ; then \
	    echo "Application <<$$app>> failed to build" ; \
	  fi ; \
	done
aprun :
	  else \
	    eval appargs=\"\\\$${$${app}_args}\" ; \
	    echo $$appargs ; \
	    if [ ! -z "$$appargs" ] ; then \
	      eval mpirun -np 4 $$app $$appargs ; \
	    fi ; \
	foo
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

info ::
	@echo "make clean       : cleanup but leave executables & slog2"
	@echo "make total_clean : cleanup including executables & slog2"
clean ::
	@/bin/rm -rf *.o *~ *.gch a.out *.dSYM MULTI__* events.* \
	    idev[0-9]*.o[0-9]* ddt.o[0-9]* jobtest.o* tautrace_*
total_clean : clean 
	@/bin/rm -rf ${APPS} *.slog2 *.ppm
