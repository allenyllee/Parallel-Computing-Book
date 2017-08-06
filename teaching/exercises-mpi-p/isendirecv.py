#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### MPI python exercise for isend/irecv
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
## Initialize local data and buffers
##
mydata = np.empty(1,dtype=np.int); mydata[0] = 1
leftdata = np.empty(1,dtype=np.int); leftdata[0] = 0
rightdata = np.empty(1,dtype=np.int); rightdata[0] = 0

## make an array to catch four requests
requests = [ None ] * 4

##
## Exercise:
## -- set `sendto' and `recvfrom' twice:
##    once to get data from the left, once from the right
## -- for first/last process use MPI_PROC_NULL as neighbour
##

## first specify communication with left neighbour
#### your code here ####
requests[
#### your code here ####
    ] = comm.Isend(mydata,sendto)
requests[
#### your code here ####
    ] = comm.Irecv(leftdata,recvfrom)

## then with right neighbour
#### your code here ####
requests[
#### your code here ####
    ] = comm.Isend(mydata,sendto)
requests[
#### your code here ####
    ] = comm.Irecv(rightdata,recvfrom)

##
## Then make sure all Isend/Irecv operations are completed
##
MPI.Request.Waitall(
#### your code here ####
    )

mydata = mydata+leftdata+rightdata
if procno==0 or procno==nprocs-1:
    if mydata!=2:
        print "Data on proc %d should be %d, not %d" % (procno,2,mydata)
else:
    if mydata!=3:
        print "Data on proc %d should be %d, not %d" % (procno,3,mydata)

if procno==0:
    print "Finished"
