info ::
	@echo "make <progname>"
	@echo "make clean"

FC = gfortran
% : %.F
	${FC} -g -o $@ $@.F
clean :
	/bin/rm -rf ${PROGS} *~ *.o *.dSYM
