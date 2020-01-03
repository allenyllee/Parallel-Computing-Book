################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-9
####
#### This is the makefile for the examples/mpi/f
####
################################################################
################################################################

info ::
	@echo "Do: make app (where app=${APPS})"

APPS = init local sendblock ssendblock sendrecv anysource \
    putfence getfence exscan \
    irecv_source reduce reduceinplace allreduce gather gatherv \
    contiguous vector struct indexed pack matchkind \
    persist typematch

listapps :
	@echo ${APPS}
listsources :
	@for a in ${APPS} ; do \
	  for e in F F90 ; do \
	    if [ -e $$a.$$e ] ; then echo $$a.$$e ; fi ; \
	  done ; \
	done | awk 'BEGIN { v="" } {v = v " " $$0} END {print v}'

ifdef TACC_TAU_DIR
  FC = tau_f90.sh
else
  FC  = mpif90
endif

OPTLEVEL = 0
F90FLAGS = -c -g -O${OPTLEVEL}

info ::
	@echo "    debug flag: DEBUG=DEBUG/OPT (default: ${DEBUG})]"
%.o : %.F90
	${FC} ${F90FLAGS} -c $*.F90 

app_dependencies =
.SECONDEXPANSION:
${APPS} : $$@.o ${app_dependencies}
	${FC} -o $@ $@.o ${app_dependencies}
${patsubst %,%.o,${APPS}} tools.o : globalinit.F90

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

include  ../../../makefiles/Make.clean
