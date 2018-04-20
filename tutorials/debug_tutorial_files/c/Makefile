info ::
	@echo "make <progname>"
	@echo "make clean"

PROGS = hello root2 say square1 root roots square square2 \
    sendrecv isendrecv isendrecv_sln bcast \
    ddt1 \
    problem2 problem3 abort \
    negative1 negative2 negative3

% : %.c
	mpicc -g -O0 -o $@ $@.c
clean :
	/bin/rm -rf ${PROGS} *~ *.o *.dSYM *.r[0-9]* ddt.o*
