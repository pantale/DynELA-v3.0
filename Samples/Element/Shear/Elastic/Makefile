# DynELA file to treat
FILE = Shear-Elastic

# Nothing to touch here after
TOP = /home/pantale/SynologyDrive/DynELA/Samples

# Store current path
HERE = $(PWD)

all:
	./runBench.py --run
	cp _bench.png $(TOP)/$(FILE).png

reference:
	./runBench.py --reference

clean:
	@echo "Cleaning: $(PWD)"
	@rm -f *.vtk *.ref *.svg *.plot *.log _bench.png

subclean: clean
	@cd $(HERE)/Abaqus; make subclean
	@cd $(HERE)/Analytic; make subclean
