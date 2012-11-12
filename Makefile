#
# Makefile for Parallel Computing for Science and Engineers
# by Victor Eijkhout
# copyright 2010/1/2
#
# complicated makefile that lets you typeset both the whole book
# and individual chapters; also exercises and answers
#
info ::
	@echo

LATEXTARGET = parcompbook
PDFFILENAME = EijkhoutIntroParComp
REPONAME = pcse-book-and-course
LATEX = pdflatex

info ::
	@echo "make book/lulu"
.PHONY: book lulu upload
book istc lulu manual ::
	cp $@.inex inex.tex
	make format
	cp ${LATEXTARGET}.pdf ${LATEXTARGET}-$@.pdf 
manual ::
	mv ${LATEXTARGET}.pdf solution-manual.pdf
# svn propset svn:keywords "Date Id" myfile.tex
format :
	export TEXINPUTS=${TEXINPUTS}:./mdchapter ; ( \
	  pdflatex ${LATEXTARGET} ; \
	  bibtex ${LATEXTARGET} ; \
	  makeindex ${LATEXTARGET} ; \
	  pdflatex ${LATEXTARGET} ; \
	  pdflatex ${LATEXTARGET} ; \
	  pdflatex ${LATEXTARGET} ; \
	)

info ::
	@echo "make upload / final_upload: put on UT website"
	@echo " (final_upload puts lulu version rather than draft)"
upload :: 
	cp ${LATEXTARGET}.pdf ${HOME}/Dropbox/Scicomp/${LATEXTARGET}.pdf
	scp ${LATEXTARGET}.pdf ve343@sftp.austin.utexas.edu:/DepartmentalDirectories/tacc/home/veijkhout/public_html/Articles/${PDFFILENAME}.pdf
final_upload :
	scp ${LATEXTARGET}-lulu.pdf ve343@sftp.austin.utexas.edu:/DepartmentalDirectories/tacc/home/veijkhout/public_html/Articles/${PDFFILENAME}.pdf

info ::
	@echo "make errata (does format and upload)"
errata :
	pdflatex errata
	scp errata.pdf ve343@sftp.austin.utexas.edu:/DepartmentalDirectories/tacc/home/veijkhout/public_html/istc

info ::
	@echo "make chapter, pdfview CHAPTER=<name>"
	@echo "  current default is $(CHAPTER)"
CHAPTERS = parallel sequential
CHAPTER = parallel
chapter :
	echo ${CHAPTER} | pdflatex chapter
	bibtex chapter ; makeindex chapter
	echo ${CHAPTER} | pdflatex chapter
	echo ${CHAPTER} | pdflatex chapter
	mv chapter.pdf handouts/$(CHAPTER).pdf
info ::
	@echo "make tutorial, pdfview TUTORIAL=<name>"
	@echo "  current default is $(TUTORIAL)"
TUTORIAL = unix
tutorial :
	echo ${TUTORIAL} | pdflatex tutorial
	bibtex tutorial ; makeindex tutorial
	echo ${TUTORIAL} | pdflatex tutorial
	echo ${TUTORIAL} | pdflatex tutorial
	mv tutorial.pdf tutorials/$(TUTORIAL).pdf
PROJECT = heat
info ::
	@echo "make project PROJECT=<name>"
	@echo "  current default is ${PROJECT}"
project :
	pdflatex project
	mv project.pdf projects/$(PROJECT).pdf

pdfview :
	xpdf -g 750x800+415+40 $(CHAPTER)-tutorial.pdf &

info ::
	@echo "make exercise/answer CHAPTER=<name>"
	@echo "  current default is $(CHAPTER)"
	@echo
exercise :
	echo $(CHAPTER) | pdflatex exercises
	mv exercises.pdf exercises/$(CHAPTER).pdf
answer :
	echo $(CHAPTER) | pdflatex answer
	mv answer.pdf answers/$(CHAPTER).pdf
info ::
	@echo "make solutions % full solutions manual"
solutions :
	pdflatex answers

info ::
	@echo "make public: source without answers"
public :
	for ext in pdf bbl ind ; do \
	  cp ${LATEXTARGET}.$$ext \
	     ../${REPONAME}/${PDFFILENAME}.$$ext ; \
	done
	cp ${LATEXTARGET}.bbl ../${REPONAME}/${PDFFILENAME}.bbl
	for f in ${LATEXTARGET} exmacs ; do \
	  cat $$f.tex \
	      | sed -e 's/answers\//answers-public\//' \
	      | sed -e 's/appendices\//appendices-public\//' \
	      | sed -e 's/chapters\//chapters-public\//' \
	      | sed -e 's/exercises\//exercises-public\//' \
	      | sed -e 's/graphics\//graphics-public\//' \
	      | sed -e 's/projects\//projects-public\//' \
	      | sed -e 's/tutorials\//tutorials-public\//' \
	      | sed -e 's/exmacs/exmacs-public/' \
	      | sed -e 's/vle/${LATEXTARGET}/' \
	  > $$f-public.tex ; \
	done
	for s in chapters tutorials appendices exercises projects ; do \
	  rm -rf $$s-public ; mkdir $$s-public ; \
	  pushd $$s ; \
	    for t in `ls *.tex` ; do \
	      cat $$t \
	      | sed -e 's/appendices\//appendices-public\//' \
	      | sed -e 's/chapters\//chapters-public\//' \
	      | sed -e 's/exercises\//exercises-public\//' \
	      | sed -e 's/graphics\//graphics-public\//' \
	      | sed -e 's/projects\//projects-public\//' \
	      | sed -e 's/tutorials\//tutorials-public\//' \
	      | awk '/begin\{answer\}/ {s=1} s==0 {print} /end{answer}/ {s=0}' \
	      > ../$$s-public/$$t ; \
	    done ; \
	  popd ; \
	done
	rm -f chapters-public/{applications,approximation,namd,performance,programming}.tex
	mkdir -p graphics-public
	cp -r graphics/*.{jpg,jpeg,png,pdf} graphics/top500 graphics-public
	cp -r tutorials/latexdemo tutorials-public
	cp ${LATEXTARGET}-public.tex ../${REPONAME}/sources/${PDFFILENAME}.tex
	cp Makefile.repo ../${REPONAME}/sources/Makefile
	cp -r ${PUBLIC_SOURCES} ../${REPONAME}/sources/
# tar fcz Eijkhout-IntroHPC-public-source.tgz ${PUBLIC_SOURCES}
PUBLIC_SOURCES = \
	  scimacs.tex exmacs-public.tex tutmacs.tex \
	  ${LATEXTARGET}.bib \
	  README \
	  copyright.tex ccby.jpg introduction.tex inex.tex \
	  graphics-public mdchapter code/papi \
	  chapters-public tutorials-public appendices-public projects-public \
	  exercises-public 


include $(MAKEFILES_DIR)/Makefile.tex
#include $(MAKEFILES_DIR)/Makefile.sync
#include $(MAKEFILES_DIR)/Makefile.clean

clean ::
	rm -f *.{aux,bbl,blg,idx,log,out,svn,toc} *~ \
	  chapter.pdf project.pdf tutorial.pdf exercises.pdf \
	  exercises/*-ex*.tex exercises/*-nx.tex answers/*-an*.tex
	for d in code/mpi ; do \
	  ( cd $$d ; make clean ) ; \
	done
