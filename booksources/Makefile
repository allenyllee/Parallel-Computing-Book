info ::
	@echo "Do: make pdf"
	@echo "to format the book"

pdf :
	pdflatex EijkhoutParComp
	pdflatex EijkhoutParComp
	pdflatex EijkhoutParComp
	cp EijkhoutParComp.pdf ../

info ::
	@echo "Do: make clean"
	@echo "to clean up temporary files"
clean ::
	@/bin/rm -f *.{aux,blg,ilg,log,out,toc} *~ comment.cut
