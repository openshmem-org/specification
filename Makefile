TARGET   = main_spec
SOURCES  = $(shell find . -name '*.tex' -print)
FIGURES  = $(shell find figures -name '*.pdf' -print)
EXAMPLES = $(shell find example_code -name '*.[c,f]*' -print)

.PHONY: all
all: ${TARGET}.pdf

${TARGET}.pdf: ${SOURCES} ${FIGURES} ${EXAMPLES}
	pdflatex $(LATEXOPT) ${TARGET}
	makeindex ${TARGET}
	pdflatex $(LATEXOPT) ${TARGET}
	makeindex ${TARGET}
	pdflatex $(LATEXOPT) ${TARGET}

.PHONY: clean
clean:
	rm -f ${TARGET}.{log,aux,ps,dvi,bbl,blg,log,idx,out,toc,pdf,out} chappage.txt

