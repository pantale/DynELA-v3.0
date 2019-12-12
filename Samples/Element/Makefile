# List of subdirectories
SUBDIRS = Shear Tensile Torus Radial

# Nothing to touch here after

# Store current path
HERE = $(PWD)

all:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make; done
	
clean:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make clean; done

subclean:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make subclean; done


