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

CLEAN_EXTENSIONS = log aux ps dvi bbl blg log idx ind ilg out toc pdf out
.PHONY: clean
clean:
	$(RM) $(addprefix ${TARGET}.,${CLEAN_EXTENSIONS}) chappage.txt
