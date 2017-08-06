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
import struct
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
## Create the output file and open for write
##
mpifile = MPI.File.Open\
          (comm,"blockwrite.dat",
           amode=MPI.MODE_CREATE+MPI.MODE_WRONLY)


##
## Exercise:
## -- set a file view so that each processor writes at a different offset
## -- note: offsets are in bytes
##
sizeofint = np.dtype('intc').itemsize
mpifile.Set_view(
#### your code here ####
    MPI.INT)

nwriters = 2
if procno<nwriters:
    mpifile.Write( [output_data,nwords,MPI.INT] )

mpifile.Close()

if procno==0:
    f = open("blockwrite.dat","rb")
    for i in range(nwords*nwriters):
        bytestring = f.read(sizeofint)
        ## python 3:
        #### fromfile = int.from_bytes( bytestring )
        ## python 2
        fromfile = struct.unpack("<L",bytestring)[0]
        if fromfile!=i+1:
            print "Error: s/b %d, got %d" % (i+1,fromfile)
    f.close()

if procno==0:
    print "Finished"
