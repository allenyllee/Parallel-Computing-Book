info ::
	@echo "Do: make app (where app=${APPS})"
APPS = helloworld helloworld-seq

% : %.c
	mpicc -o $@ $^

info ::
	@echo "    make clean"
clean ::
	/bin/rm -rf *.o *~ a.out *.dSYM ${APPS}
