info ::

APPS = time_avg

ifdef TACC_TAU_DIR
  CC = tau_cc.sh
  CXX = tau_cxx.sh
else
  CC  = mpicc
  CXX = mpicxx
endif

OPT_level = 2
OPT_flag = -O${OPT_level} -g
info ::
	@echo "Do: make app (where app=${APPS})"
% : %.c
	${CC} -std=c99 ${OPT_flag} $*.c -o $@
%.o : %.cxx
	${CXX} ${OPT_flag}} -c $*.cxx

# rules for submitting to the queue and doing tau analysis
info ::
	@echo
	@echo "make submit EXECUTABLE=<any prog> OPTIONS=<whatever>"
EXECUTABLE = time_avg
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
idevrun :
	@if [ -z "${EXECUTABLE}" ] ; then \
	   echo "Usage: make ibrun executable option"; exit 1 ; fi
	@export TAU_EXT="_${EXECUTABLE}" ; \
	export TAU_DUMP_DIR=`pwd`/tautrace$$TAU_EXT ; \
	  rm -rf $${TAU_DUMP_DIR}; mkdir -p $${TAU_DUMP_DIR} ; \
	  TAU_TRACE=1 TAU_PROFILE=1 \
	  TRACEDIR=$${TAU_DUMP_DIR} \
	  PROFILEDIR=$${TAU_DUMP_DIR} \
	    ibrun ${EXECUTABLE} ${OPTIONS}
info ::
	@echo "make tau EXECUTABLE=..."
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
	@echo "make regression"
regression: total_clean
	for a in ${MANDEL_APPS} ; do \
	  echo "Compilation $$a" ; \
	  make $$a >/dev/null 2>&1 ; \
	  if [ ! -e $$a ] ; then echo ".. could not make $$a" ; fi ; \
	  if [ ! -z "${SLURM_NNODES}" ] ; then \
	    echo ".. run $$a" ; \
	    rm -f mandelpicture* ; \
	    ibrun $$a >$$a.runlog 2>&1 ; \
	    if [ ! -e "mandelpicture.ppm" ] ; then \
	      echo ".. failed to run $$a" ; \
	    fi ; \
	  fi ; \
	done

info ::
	@echo "make clean       : cleanup but leave executables & slog2"
	@echo "make total_clean : cleanup including executables & slog2"
clean ::
	/bin/rm -rf *.o *~ *.gch a.out *.dSYM MULTI__* events.* *.runlog \
	    core.[0-9]* idev[0-9]*.o[0-9]* ddt.o[0-9]* jobtest.o* tautrace_*
total_clean : clean 
	/bin/rm -rf ${APPS} *.slog2 *.ppm
