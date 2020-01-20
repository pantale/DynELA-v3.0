# DynELA file to treat
FILE = Shear-Elastic

# Pictures for documentation
GENERIC = $(FILE)
OUTPUTS = stress_11.svg stress_12.svg stress_22.svg vonMises.svg
OUTDIR = $(DYNELA)/Documentation/Figures/Samples/Element

# Nothing to touch here after
TOP = $(DYNELA)/Samples

# Store current path
HERE = $(PWD)

all:
	./runBench.py --run
	cp _bench.png $(TOP)/$(FILE).png

reference:
	./runBench.py --reference

install:
	@for OUTPUT in $(OUTPUTS); do rsync --checksum --no-times $$OUTPUT $(OUTDIR)/$(GENERIC)_$$OUTPUT; done

clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.vtk *.ref *.svg *.plot *.log _bench.png

subclean: clean
	@cd $(HERE)/Abaqus; make subclean
	@cd $(HERE)/Analytic; make subclean
