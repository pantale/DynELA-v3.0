# DynELA file to treat
FILE = Taylor

# Pictures for documentation
GENERIC = Taylor-3D
OUTPUTS = plasticStrainCP.svg temperatureCP.svg radius.svg height.svg temperature.svg plasticStrain.svg vonMises.svg timeStep.svg mesh.svg
OUTDIR = $(DYNELA)/Documentation/Figures/Samples/Impact

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


