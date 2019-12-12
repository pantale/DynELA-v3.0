TOP = DynELA.pdf

# No modification below this line
SOURCES = $(wildcard *.lyx)
MAINTOP = $(TOP:.pdf=.lyx)

all : $(TOP)

$(TOP) : $(SOURCES) pageGarde.pdf
	@rm -f $(TOP)
	lyx -e pdf2 $(MAINTOP)

clean: cleanTex
	@rm -f  *.dvi *.aux *.log *.toc *~ *.lof *.lot *.ind *.ilg *.idx *.bbl *.blg *.brf *.loa *.lop *.out

cleanTex:
	@rm -f *.tex

force:
	@rm -f $(TOP)	
	make $(TOP)
