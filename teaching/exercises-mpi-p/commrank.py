#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### hello.py : just init and hello world
####
################################################################
################################################################

from mpi4py import MPI

comm = MPI.COMM_WORLD

####
#### Exercise:
#### - use the Get_size and Get_rank methods of the COMM class
#### - let each processor print out a message like
####   "Hello from processor 7 out of 12"
####

#### your code here ####

####
#### Exercise:
#### - print out the number of processes from process zero
####
if procno==0:
    print "Running with %d processes" % nprocs


