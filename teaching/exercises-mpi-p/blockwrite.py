#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### MPI python exercise for MPI I/O
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## Each process has a buffer of length `nwords'
## containing consecutive integers
##
nwords = 3
output_data = np.empty(nwords,dtype=np.intc)
for iw in range(nwords):
    output_data[iw] = procno*nwords + iw + 1

##
## Exercise:
## -- open a file for writing;
##    if it doesn't exist yet, it needs to be created
##
mpifile = MPI.File.Open \
          (comm,"blockwrite.dat",
#### your code here ####
          )

##
## Exercise:
## -- Let each process compute an offset (in bytes!) so that the
##    processes write to disjoint, successive parts of the file.
##
nwriters = 2
if procno<nwriters:
    ##
    ## Set the right offset, in ints
    ##
#### your code here ####
    ## convert to bytes by multiplying by the size of an int
    offset = offset * np.dtype('intc').itemsize

    mpifile.Write_at \
        (offset,output_data)

mpifile.Close()

if procno==0:
    print "Finished"
