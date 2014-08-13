################################################################
################################################################
####
#### This make file is part of the source of 
#### `Parallel Programing in Science and Engineering'
#### by Victor Eijkhout, copyright 2013/4
####
#### This is a makefile for the programming exercises
####
################################################################
################################################################

info ::
	@echo "make"

APPS = 

ifdef TACC_TAU_DIR
  FC = tau_f77.sh
  F90 = tau_f90.sh
  CC = tau_cc.sh
  CXX = tau_cxx.sh
else
  FC  = mpif77
  F90 = mpif90
  CC  = mpicc
  CXX = mpicxx
endif

OPTLEVEL = 2
FFLAGS = -g -O${OPTLEVEL}
CFLAGS = ${FFLAGS} -std=c99 
info ::
	@echo "    [OPTLEVEL=... (default: ${OPTLEVEL})]"
% : %.F
	${FC} ${FFLAGS} $*.F -o $@
%.o : %.F90
	${F90} ${FFLAGS} -c $*.F90
%.o : %.cxx
	${CXX} ${CFLAGS} -c $*.cxx

# Example: mandelbrot routines
MANDEL_APPS = mandel_serial mandel_bulk mandel_async mandel_collective
APPS += ${MANDEL_APPS}
mandel_dependencies = mandel_tools.o imagetools.o Image.o
mandel_tools.o : Image.o
${MANDEL_APPS} : ${mandel_dependencies}
APPS += ${MANDEL_APPS}
Color.o : Color.h
Image.o : Image.h
.SECONDEXPANSION:
${MANDEL_APPS} : ${mandel_dependencies} $$@.o
	${F90} ${FFLAGS} $@.o ${mandel_dependencies}  -lstdc++ -lc -o $@
# -lstdc++ -lc

# rules for submitting to the queue and doing tau analysis
info ::
	@echo
	@echo "make submit EXECUTABLE=<any prog> OPTIONS=<whatever>"
EXECUTABLE = grid_blocking
OPTIONS = pi 4 pj 6 ni 400 nj 600 it 10
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
	@echo "make idevrun EXECUTABLE=<any prog> OPTIONS=<whatever>"
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
	@echo "make tau EXECUTABLE=... (this does the post-processing)"
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
	@echo "make clean       : cleanup but leave slog2"
	@echo "make total_clean : cleanup including slog2"
include ../../Make.inc
clean :: codeclean
total_clean : clean 
	@/bin/rm -rf ${APPS} *.slog2 *.ppm
