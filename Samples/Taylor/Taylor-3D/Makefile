# DynELA file to treat
FILE = Taylor-3D

# Nothing to touch here after
TOP = /home/pantale/SynologyDrive/DynELA/Samples

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


