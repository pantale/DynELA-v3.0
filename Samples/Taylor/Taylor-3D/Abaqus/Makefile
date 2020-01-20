# Abaqus file to treat
FILE = Taylor

# Pictures for documentation
GENERIC = Taylor-3D-Abaqus
OUTPUTS = PlasticStrain.svg Temperature.svg
OUTDIR = $(DYNELA)/Documentation/Figures/Samples/Impact

# Nothing to touch here after
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
	@for OUTPUT in $(OUTPUTS); do rsync --checksum --no-times $$OUTPUT $(OUTDIR)/$(GENERIC)_$$OUTPUT; done

clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.odb *.plot

subclean: clean

