#
# DynELA Finite Element Code Makefile
#
#@!CODEFILE = Makefile-sample-abaqus
#
# -------------------------------------------------------------------------------
# Start of Private zone for the Makefile
# -------------------------------------------------------------------------------
#@!BEGIN = PRIVATE

# Abaqus file to treat
FILE = Tensile

#@!BEGIN = COMMON
# -------------------------------------------------------------------------------
# There is nothing to touch here after
# If there is not any changes to make, this has to be done in tha master Makefile
# -------------------------------------------------------------------------------
all:
	abaqus job=$(FILE) input=$(FILE) double=both interactive ask_delete=OFF
	abaqus python Extract.py
	@echo "Sleep for 2 seconds for files sync"
	@sleep 2
	@rm -f abaqus.rpy*
	replaceInFile "$(FILE)_*.plot" $(FILE)_ Abaqus-
	@rm -f sauvegarde_replaceInFile.tar.gz
	@rm -f $(FILE)*.abq $(FILE)*.com $(FILE)*.log $(FILE)*.mdl $(FILE)*.msg $(FILE)*.pac $(FILE)*.prt $(FILE)*.res $(FILE)*.sel $(FILE)*.stt $(FILE)*.dat $(FILE)*.sta

install:
	@svgToPdf -dest $(OUTDIR) -prefix $(GENERIC)_

clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.odb *.plot

subclean: clean

