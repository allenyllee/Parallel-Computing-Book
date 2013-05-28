info ::
	@echo "make prog"
	@echo "  where prog is one of: ${PROGS}"

include ${PETSC_DIR}/conf/variables

PROGS = init vec set mat 
.SECONDEXPANSION:
%.o : %.F
	-${FC} -c -o $@ $^ ${PETSC_FC_INCLUDES}
${PROGS}: $$@.o 
	-${FLINKER} -o $@ $@.o ${PETSC_LIB}

clean :
	rm -f *~ *.o ${PROGS}
