################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
################################################################
################################################################

info ::
	@echo

LATEXTARGET = parcompbook
PDFFILENAME = EijkhoutParComp
LATEX = pdflatex

info ::
	@echo "make book/pcse/lulu/manual"
	@echo "   (make format, onepass)"
.PHONY: book lulu upload
book pcse lulu manual ::
	cp $@.inex inex.tex
	make format
	cp ${LATEXTARGET}.pdf ${LATEXTARGET}-$@.pdf 
	make upload
book::
	cp ${LATEXTARGET).pdf ${PDFFILENAME}.pdf
manual ::
	cp ${LATEXTARGET}.pdf solution-manual.pdf
# svn propset svn:keywords "Date Id" myfile.tex
onepass :
	TEXINPUTS=${TEXINPUTS}:chapters:graphics:life/graphics:../scicompbook/graphics pdflatex ${LATEXTARGET}
format : snippets
	make onepass
	make exercises_extract
	bibtex ${LATEXTARGET}
	make onepass
	make onepass
	makeindex ${LATEXTARGET}
	make onepass

info ::
	@echo "make snippets"
.PHONY: snippets
snippets :
	export pwd=`pwd` ; \
	cd snippets ; \
	  for d in code examples ; do \
	    for x in $$pwd/$$d/mpi/c/*.{c,cxx,h} $$pwd/$$d/mpi/f/*.F90 \
	             $$pwd/$$d/omp/c/*.c         $$pwd/$$d/omp/f/*.F90 ; do \
	      cat $$x | awk -v fn=$${x##*/} \
	                'BEGIN {f=0; ff=0} \
	                 /pragma end/ {f=0} \
	                 /snippet end/ {f=0} \
			 f==1 && ff==0 {print "// " fn > file; ff=1} \
	                 f==1 {print $$0 > file} \
	                 /pragma/ && $$2!="omp" {f=1; file=$$2 } \
	                 /snippet/ {f=1; file=$$2 } \
	                ' ; \
	    done ; \
	  done ; \
	  for s in `ls` ; do \
	    python ../cutspace.py $$s ; \
	  done

info ::
	@echo "make upload / final_upload: put on UT website"
	@echo " (final_upload puts lulu version rather than draft)"
upload :: 
	cp ${LATEXTARGET}.pdf ${HOME}/Dropbox/Scicomp/${LATEXTARGET}.pdf
#	scp ${LATEXTARGET}.pdf ve343@sftp.austin.utexas.edu:/DepartmentalDirectories/tacc/home/veijkhout/public_html/Articles/${PDFFILENAME}.pdf
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
	@echo "make exercise FILE=<name>"
	@echo "make exercises/answer CHAPTER=<name>"
	@echo "  current default is $(CHAPTER)"
	@echo
.PHONY: exercise exercises_extract
exercise :
	echo $(FILE) | \
	    TEXINPUTS=${TEXINPUTS}:graphics:life/graphics pdflatex exercise
	echo $(FILE) | \
	    TEXINPUTS=${TEXINPUTS}:graphics:life/graphics pdflatex exercise_and_solution
exercises_extract :
	cd exercises ; \
	for e in *.tex ; do \
	  if [ `cat $$e | grep label | wc -l` -eq 1 ] ; then \
	    export nam=`cat $$e | grep label | sed -e s/^.*label.// -e s/\}//` ; \
	    cp $$e $$nam.tex ; \
	  fi ; \
	done
# echo $(CHAPTER) | pdflatex exercises
# mv exercises.pdf exercises/$(CHAPTER).pdf
answer :
	echo $(CHAPTER) | pdflatex answer
	mv answer.pdf answers/$(CHAPTER).pdf
info ::
	@echo "make solutions % full solutions manual"
solutions :
	pdflatex answers

info ::
	@echo "make unittests: do some testing of the example programs"
unittests :
	cd examples/mpi/c ; make regression

info ::
	@echo "make public: source without answers"
REPONAME = pcse-book-and-course
public : 
	@export PUBLICDIR=`pwd`/../${REPONAME}/booksources ; \
	 mkdir $$PUBLICDIR/exercises ; \
	 echo "This directory contains only automatically generated files" > $$PUBLICDIR/exercises/README ;\
	 echo " .. main files" ; \
	 cp README $$PUBLICDIR/.. ; \
	 cp boek3.cls copyright.tex introduction.tex $$PUBLICDIR ; \
	 cp Makefile.repo $$PUBLICDIR/Makefile ; \
	 cp {exmacs,scimacs,tutmacs}.tex $$PUBLICDIR ; \
	 for ext in tex pdf bbl ind ; do \
	   cp ${LATEXTARGET}.$$ext $${PUBLICDIR}/${PDFFILENAME}.$$ext ; \
	 done ; \
	 cp ${LATEXTARGET}.pdf $${PUBLICDIR}/../${PDFFILENAME}.pdf ; \
	 cp book.inex $${PUBLICDIR}/inex.tex ; \
	 echo " .. graphics and other includes"; \
	 mkdir -p $$PUBLICDIR/graphics ; \
	 ( cd graphics ; cp *.{jpg,jpeg,png} $${PUBLICDIR}/graphics ) ; \
	 echo " .. TeX files" ; \
	 for s in chapters tutorials appendices ; do \
	  echo " .. .. in $$s" ; \
	  mkdir -p $$PUBLICDIR/$$s ; \
	  ( cd $$s ; \
	    for t in *.tex ; do \
	      cat $$t | awk '/begin\{answer\}/ {s=1} s==0 {print} /end{answer}/ {s=0}' \
	      > $${PUBLICDIR}/$$s/$$t ; \
	    done ; \
	  ) ; \
	 done ; \
	echo " .. small stuff" ; \
	mkdir -p $$PUBLICDIR/projects ; \
	( cd projects ; cp *.tex $$PUBLICDIR/projects ) ; \
	mkdir -p $$PUBLICDIR/code/papi ; \
	( cd code/papi ; cp Makefile *.c $$PUBLICDIR/code/papi ) ; \
	mkdir -p $$PUBLICDIR/snippets ; cp -r snippets/* $$PUBLICDIR/snippets
oldoldpublic :
	 cp {exmacs,inex,scimacs,tutmacs,copyright,introduction}.tex $$PUBLICDIR ; \
	 cp ${LATEXTARGET}.pdf $${PUBLICDIR}/../${PDFFILENAME}.pdf ; \
	 for ext in tex pdf bbl ind ; do \
	   cp ${LATEXTARGET}.$$ext $${PUBLICDIR}/${PDFFILENAME}.$$ext ; \
	 done ; \
	 for s in appendices chapters projects tutorials ; do \
	  echo "..  in $$s" ; \
	  export REPODIR=$$PUBLICDIR/$$s ; mkdir -p $$REPODIR ; \
	  ( cd $$s ; \
	    for t in *.tex ; do \
	      cat $$t \
	      | awk '/begin\{answer\}/ {s=1} s==0 {print} /end{answer}/ {s=0}' \
	      > $$REPODIR/$$t ; \
	    done ; ) ; \
	 done ; \
	 export targetdir=$$PUBLICDIR/chapters/mpiref ; mkdir -p $$targetdir ; \
	 cp chapters/mpiref/*.tex $$targetdir ; \
	 echo ".. snippets" ; \
	 export targetdir=$$PUBLICDIR/snippets ; \
	 mkdir -p $$targetdir ; \
	 cp snippets/* $$targetdir/ ; \
	 echo ".. projects" ; \
	 export targetdir=$$PUBLICDIR/projects ; \
	 mkdir -p $$targetdir ; \
	 cp -r projects/* $$targetdir/ ; \
	 echo ".. graphics" ; \
	 export targetdir=$$PUBLICDIR/graphics ; \
	 mkdir -p $$targetdir ; \
	 cp -r graphics/*.{jpg,jpeg,png} $$targetdir/ ; \
	 echo ".. examples" ; \
	 for l in c f p ; do \
	   export targetdir=$$PUBLICDIR/examples/mpi/$$l ; \
	   mkdir -p $$targetdir ; \
	   ( cd examples/mpi/$$l ; \
	        make regression ; \
	        for f in `make listsources` Makefile ; do \
	          echo ".... strip $$f" ; \
	          cat $$f | sed '/pragma/d' > $$targetdir/$$f ; \
	        done ; \
	   ) ; \
	 done
prevpublic :
	@echo ".. coding projects"
	@export targetdir=$$PUBLICDIR/projects/mpi/c ; \
	 mkdir -p $$targetdir ; \
	    cd code/mpi/c ; \
	    for f in \
	        Makefile jobscript README \
	        helloworld.c time_max.cxx \
		mandel_serial.cxx mandel_tools.cxx mandel.h tools.cxx tools.h \
	        Image.cxx Image.h Color.cxx Color.h \
		; do \
	      echo ".... strip $$f" ; \
	      cat $$f | sed '/#pragma/d' > $$targetdir/$$f ; \
	    done
	@export targetdir=$$PUBLICDIR/projects/mpi/f ; \
	 mkdir -p $$targetdir ; \
	    cd code/mpi/f ; \
	    for f in \
	        README Makefile jobscript\
	        mandel_serial.F90 mandel_tools.F90 \
	        Image.cxx Image.h Color.cxx Color.h \
		; do \
	      echo ".... strip $$f" ; \
	      cat $$f | sed '/#pragma/d' > $$targetdir/$$f ; \
	    done

# tar fcz Eijkhout-IntroHPC-public-source.tgz ${PUBLIC_SOURCES}
PUBLIC_SOURCES = \
	  scimacs.tex exmacs-public.tex tutmacs.tex \
	  README \
	  copyright.tex introduction.tex inex.tex
PUBLIC_DIRS = \
	  appendices-public examples-public exercises-public \
	  chapters-public graphics-public tutorials-public  \
	  projects-public/mpi/c projects-public/mpi/f \
	  snippets-public

NO_SOURCES = \
	  ${LATEXTARGET}.bib \
	  ccby.jpg

#include $(MAKEFILES_DIR)/Makefile.tex

include Make.inc
clean :: localclean
	@for d in code examples projects quizes test ; do \
	  ( cd $$d ; echo "cleaning $$d" ; make clean ) ; \
	done
	@rm -f \
	  comment.cut \
	  chapter.pdf project.pdf tutorial.pdf exercises.pdf \
	  exercises/*-ex*.tex exercises/*-nx.tex answers/*-an*.tex
	-@for d in code/mpi/c \
	  tutorials/debug_tutorial_files/c tutorials/debug_tutorial_files/f \
	  life \
	  ; do \
	  ( cd $$d ; make clean ) ; \
	done
