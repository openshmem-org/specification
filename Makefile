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

RM ?= rm -f
CLEAN_EXTENSIONS = aux idx ilg ind log out pdf toc
.PHONY: clean
clean:
	for e in ${CLEAN_EXTENSIONS}; do $(RM) ${TARGET}.$$e; done
