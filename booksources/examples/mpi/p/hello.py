#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### hello.py : just init and hello world
####
################################################################
################################################################

from mpi4py import MPI

comm = MPI.COMM_WORLD

ntids = comm.Get_size()
mytid = comm.Get_rank()

if mytid==0:
    print "Starting %d processes" % ntids

