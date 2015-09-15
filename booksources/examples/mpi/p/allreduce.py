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

myrandom = np.empty(1,dtype=np.int)
myrandom[0] = random.randint(1,nprocs*nprocs)
allrandom = np.empty(nprocs,dtype=np.int)

print "[%d] random=%d" % (procid,myrandom[0])

comm.Allreduce(myrandom,allrandom[:1],op=MPI.MAX)
print "[%d] max=%d" % (procid,allrandom[0])
