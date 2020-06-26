#
# DynELA Finite Element Code Makefile
#
#@!CODEFILE = Makefile-sample-subdir
#
# -------------------------------------------------------------------------------
# Start of Private zone for the Makefile
# -------------------------------------------------------------------------------
#@!BEGIN = PRIVATE

# List of subdirectories to take into account
SUBDIRS = ElQua4N2D ElHex8N3D

#@!BEGIN = COMMON
# -------------------------------------------------------------------------------
# There is nothing to touch here after
# If there is not any changes to make, this has to be done in tha master Makefile
# -------------------------------------------------------------------------------

# Store current path
HERE = $(PWD)

all:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make; done
	
clean:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make clean; done

subclean:
	@for SUBDIR in $(SUBDIRS); do cd $(HERE)/$$SUBDIR; make subclean; done


