#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-8
####
#### blockwrite.py : MPI python exercise for MPI I/O
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
filename = "blockwrite.dat"
mpifile = MPI.File.Open \
          (comm,filename,
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

##
## Check correctness of the output file
##
if procno==0:
    error = 0; errors = np.full(nprocs,nprocs,np.int)
    # with open(filename,"r") as timefile:
    #     location = 0
    #     for ip in range(nwriters):
    #         for iw in range(nwords):
    #             line = timefile.readline()
    #             print(line)
    if errors[0]==nprocs:
        print("Finished: (read not implemented in python)")
    else:
        print("Finished: first error occurred on rank %d" % errors[0])
