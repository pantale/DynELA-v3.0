# DynELA file to treat
FILE = Radial

# Pictures for documentation
GENERIC = $(FILE)
OUTPUTS = plasticStrain.svg stress_11.svg stress_12.svg stress_22.svg temperature.svg vonMises.svg
OUTDIR = $(DYNELA)/Documentation/Figures/Samples/Element

# Nothing to touch here after
TOP = $(DYNELA)/Samples

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
	@cd Abaqus; make subclean

