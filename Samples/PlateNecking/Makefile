# DynELA file to treat
FILE = PlateNecking

# Nothing to touch here after
TOP = $(DYNELA)/Samples

all:
	./runBench.py --run
	cp _bench.png $(TOP)/$(FILE).png

reference:
	./runBench.py --reference

clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.vtk *.ref *.svg *.plot *.log _bench.png

subclean: clean
	@cd Abaqus; make subclean


