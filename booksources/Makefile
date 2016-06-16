# -*- makefile -*-
################################################################
################################################################
####
#### This is the makefile for the book
#### `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2012-6
####
################################################################
################################################################

info ::
	@echo "Do: make pdf"
	@echo "to format the book"

pdf :
	TEXINPUTS=${TEXINPUTS}:graphics:mpireference pdflatex EijkhoutParComp
	TEXINPUTS=${TEXINPUTS}:graphics:mpireference pdflatex EijkhoutParComp
	TEXINPUTS=${TEXINPUTS}:graphics:mpireference pdflatex EijkhoutParComp
	cp EijkhoutParComp.pdf ../
clean ::
	@rm -f EijkhoutParComp.pdf acronyms.tex chapternames.tex

info ::
	@echo "Do: make clean"
	@echo "to clean up temporary files"
clean ::
	@/bin/rm -f *.{aux,blg,idx,ilg,log,out,toc} *~ comment.cut \
	 exercises/*.tex
