################################################################
################################################################
####
#### This is the makefile for the book
#### `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2012/3
####
################################################################
################################################################

info ::
	@echo "Do: make pdf"
	@echo "to format the book"

pdf :
	pdflatex EijkhoutParComp
	pdflatex EijkhoutParComp
	pdflatex EijkhoutParComp
	cp EijkhoutParComp.pdf ../
clean ::
	@rm -f EijkhoutParComp.pdf acronyms.tex chapternames.tex

info ::
	@echo "Do: make clean"
	@echo "to clean up temporary files"
clean ::
	@/bin/rm -f *.{aux,blg,idx,ilg,log,out,toc} *~ comment.cut \
	 exercises/*-nx.tex exercises/*-ex*.tex
