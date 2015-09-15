info ::
	@echo "make clean"

include ../Make.inc
clean :: localclean
	@for d in mpi/c ; do \
	  echo "cleaning $$d" ; \
	  ( cd $$d ; make total_clean ) ; \
	done
