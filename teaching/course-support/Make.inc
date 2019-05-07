# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### Victor Eijkhout
#### eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-8
####
#### Makefile for short course OpenMPtemplates
####
################################################################

OMPFLAGS_intel    = -g -qopenmp 
OMPFLAGS_gcc      = -ggdb -fopenmp
OMPFLAGS = ${OMPFLAGS_${TACC_FAMILY_COMPILER}}

info ::
	@echo "    [ OPTLEVEL=0/1/2, default:${OPTLEVEL} ]"
OPTLEVEL = 0

info ::
	@echo "    [ EXTRA_OPTIONS=... ]"
EXTRA_OPTIONS =

ifdef TACC_TAU_DIR
  C_COMPILER = tau_cc.sh
  CXX_COMPILER = tau_cxx.sh
  F_COMPILER = tau_f90.sh
  MPI_C_COMPILER = tau_cc.sh
  MPI_CXX_COMPILER = tau_cxx.sh
  MPI_F_COMPILER = tau_f90.sh
else
  C_COMPILER_intel = icc
  C_COMPILER_gcc   = gcc
  CFLAGS = ${OMPFLAGS} -O${OPTLEVEL} -std=c99 ${EXTRA_OPTIONS}
  C_COMPILER = ${C_COMPILER_${TACC_FAMILY_COMPILER}}
  MPI_C_COMPILER = mpicc

  CXX_COMPILER_intel = icpc
  CXX_COMPILER_gcc   = g++
  CXXFLAGS = -O${OPTLEVEL} -std=c++11 ${EXTRA_OPTIONS}
  CXX_COMPILER = ${CXX_COMPILER_${TACC_FAMILY_COMPILER}}
  MPI_CXX_COMPILER = mpicxx

  F_COMPILER_intel = ifort
  F_COMPILER_gcc   = gfortran
  FFLAGS = ${OMPFLAGS} -O${OPTLEVEL} ${EXTRA_OPTIONS}
  F_COMPILER = ${F_COMPILER_${TACC_FAMILY_COMPILER}}
  MPI_F_COMPILER = mpif90
endif

clean ::
	@/bin/rm -rf *~ MULTI*
