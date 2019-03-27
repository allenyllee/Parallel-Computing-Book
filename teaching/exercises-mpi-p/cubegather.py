#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-8
####
#### cubegatherp.py : use subarray type
####
################################################################
################################################################

from mpi4py import MPI
import math
import numpy as np
import random
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## See if you can arrange the processes
## in a perfect cube. If not, quit.
##
procs_per_side = int( math.pow(nprocs+1,1./3) )
if pow(procs_per_side,3) != nprocs:
    if procno==0:
        print("Number of processors needs to be a perfect square")
    comm.Barrier()
    comm.Abort(0)

##
## Every process gets its own data and sends it to process zero.
## Exercise: why are we using Isend here?
