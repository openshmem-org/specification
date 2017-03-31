TARGET=main_spec

# make pdf by default
all: ${TARGET}.pdf

# it doesn't really need the .dvi, but this way all the refs are right
%.pdf : %.dvi
	pdflatex $(LATEXOPT) $*

${TARGET}.bbl: ${TARGET}.bib
# in case we don't already have a .aux file listing citations
# this should probably be a separate makefile target/dependency instead
# of doing it every time... but *shrug*
	pdflatex $(LATEXOPT) ${TARGET}.tex
# get the citations out of the bibliography
	bibtex $(TARGET)
# do it again in case there are out-of-order cross-references
	@pdflatex $(LATEXOPT) ${TARGET}.tex

#${TARGET}.dvi: ${TARGET}.bbl ${TARGET}.tex
${TARGET}.dvi: ${TARGET}.tex
	@pdflatex $(LATEXOPT) ${TARGET}.tex

# shortcut, so we can say "make ps"
ps: ${TARGET}.ps

${TARGET}.ps: ${TARGET}.dvi
	@dvips -t a4 ${TARGET}.dvi

clean:
	rm -f ${TARGET}.{log,aux,ps,dvi,bbl,blg,log,idx,out,toc} chappage.txt

veryclean: clean
	rm -f ${TARGET}.{ps,pdf,out}


PHONY : ps all clean reallyclean

