# -*- makefile -*-
################################################################
################################################################
####
#### This is the makefile for the book
#### `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2012-2020
####
################################################################
################################################################

info ::
	@echo "Do: make pdf"
	@echo "to format the book"

onepass :
	TEXINPUTS=.:graphics//:mpireference:snippets:macros:graphics:hwloc:..//:${TEXINPUTS} \
	    pdflatex EijkhoutParComp
pdf :
	make onepass
	make onepass
	make onepass
	cp EijkhoutParComp.pdf ../
clean ::
	@rm -f EijkhoutParComp.pdf acronyms.tex chapternames.tex

info ::
	@echo "Do: make clean"
	@echo "to clean up temporary files"
clean ::
	@/bin/rm -f *.{aux,blg,idx,ilg,ind,log,out,toc} *~ comment.cut \
	 exercises/*.tex
