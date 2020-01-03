################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013/4/5
####
#### Makefile for OpenMP C example programs
####
################################################################
################################################################

info ::
	@echo ; echo "OpenMP example programs" ; echo ; echo "available commands:"

include Make.inc

info ::
	@echo
	@echo "make app (where app=${APPS})"
APPS = hello hellocount
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
  FC  = ${F90_COMPILER}
endif
info ::
	@echo "    [OPTLEVEL=0/1/2/3 (default=${OPTLEVEL})]"
% : %.F90
	${FC} ${F90_OPTFLAGS} $*.F90 -o $@

EXEC = hello
NP = 4
info ::
	@echo
	@echo "make testrun [EXEC=app (where app in ${APPS})] [NP=nnn (default: ${NP})]"
testrun:
	make clean; rm -f ${EXEC}; make ${EXEC}
	OMP_NUM_THREADS=${NP} ./${EXEC}

include  ../../../makefiles/Make.clean
