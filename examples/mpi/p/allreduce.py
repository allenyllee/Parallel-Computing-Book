#!/usr/bin/env python

################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### allreduce.py : note the necessity for equal send/recv 
####     buffer sizes
####
################################################################
################################################################

import numpy as np
import random
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print "C'mon, get real...."
    sys.exit(1)

random_number = random.randint(1,nprocs*nprocs)
print "[%d] random=%d" % (procid,random_number)

max_random = comm.allreduce(random_number,op=MPI.MAX)
if procid==0:
    print "Python native:\n  max=%d" % max_random

myrandom = np.empty(1,dtype=np.int)
myrandom[0] = random_number
allrandom = np.empty(nprocs,dtype=np.int)

comm.Allreduce(myrandom,allrandom[:1],op=MPI.MAX)

if procid==0:
    print "Python numpy:\n  max=%d" % allrandom[0]

