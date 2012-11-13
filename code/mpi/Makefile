info ::
	@echo "Do: make app (where app=${APPS})"
APPS = helloworld helloworld-seq

ifdef TACC_TAU_DIR
  CC = tau_cc.sh
else
  CC = mpicc
endif

% : %.c
	${CC} -o $@ $^

info ::
	@echo "    make clean"
clean ::
	/bin/rm -rf *.o *~ a.out *.dSYM ${APPS}
