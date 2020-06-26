#
# DynELA Finite Element Code Makefile
#
#@!CODEFILE = Makefile-sample-dynela
#
# -------------------------------------------------------------------------------
# Start of Private zone for the Makefile
# -------------------------------------------------------------------------------
#@!BEGIN = PRIVATE

# DynELA file to treat
FILE = Tensile

# Pictures for documentation
GENERIC = Tensile3D
OUTPUTS = plasticStrain.svg stress_11.svg stress_12.svg stress_22.svg temperature.svg vonMises.svg
OUTDIR = $(DYNELA)/Documentation/Figures/Samples/Element

#@!BEGIN = COMMON
# -------------------------------------------------------------------------------
# There is nothing to touch here after
# If there is not any changes to make, this has to be done in tha master Makefile
# -------------------------------------------------------------------------------

TOP = $(DYNELA)/Samples

all:
	./runBench.py --run
	cp _bench.png $(TOP)/$(FILE).png

reference:
	./runBench.py --reference

install:
	@svgToPdf -dest $(OUTDIR) -prefix $(GENERIC)_

clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.vtk *.ref *.svg *.plot *.log _bench.png

subclean: clean
	@cd Abaqus; make subclean


