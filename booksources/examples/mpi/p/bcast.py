#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### bcast.py : broadcast of numpy float
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import sys

comm = MPI.COMM_WORLD

procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print "C'mon, get real...."
    sys.exit(1)

root = 1
dsize = 10
buffer = np.arange(dsize, dtype=np.float64)

if procid==root:
    for i in range(dsize):
        buffer[i] = 5.0
comm.Bcast( buffer,root=root )
if not all( buffer==5.0 ):
    print "Something wrong on proc %d" % procid
    
