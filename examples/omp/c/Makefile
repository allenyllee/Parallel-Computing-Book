################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2013-8
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
APPS = hello hellocount \
    alloc1 alloc2 sharing \
    threadprivate ireduct ireductinit ureduct reductpositive \
    taskgroup0 taskgroup1 taskgroup2 taskgroup3
listapps :
	@echo ${APPS}
listsources :
	@for a in ${APPS} ; do \
	  for e in c cxx ; do \
	    if [ -e $$a.$$e ] ; then echo $$a.$$e ; fi ; \
	  done ; \
	done | awk 'BEGIN { v="" } {v = v " " $$0} END {print v}'

ifdef TACC_TAU_DIR
  CC = tau_cc.sh
  CXX = tau_cxx.sh
else
  CC  = ${C_COMPILER}
  CXX = ${CXX_COMPILER}
endif

info ::
	@echo "    [OPTLEVEL=0/1/2/3 (default=${OPTLEVEL})]"
	@echo "    [EXTRAFLAGS=whatever]"
EXTRAFLAGS =
% : %.c
	${CC}  ${CFLAGS}   ${EXTRAFLAGS} $*.c -o $@
% : %.cxx
	${CXX} ${CXXFLAGS} ${EXTRAFLAGS} $*.cxx -o $@

EXEC = hello
NP = 4
info ::
	@echo
	@echo "make testrun [EXEC=app (where app in ${APPS})] [NP=nnn (default: ${NP})]"
testrun:
	make clean; rm -f ${EXEC}; make ${EXEC}
	OMP_NUM_THREADS=${NP} ./${EXEC}

include ../../Make.inc
