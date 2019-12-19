# DynELA Finite Element Code v.3.0

This repository contains the DynELA Finite Element Code v.3.0. The DynELA Finite Element Code is an Explicit FEM code written in C++ using a Python's interface for creating the FEM models. This is a new version of the early proposed v.2 code written between 1996 and 2010. The previous version is included into the CAE Linux distribution. The aim of v.3.0 is to provide a new version of the code, along with some documentation.

## Download and installation procedure:

The DynELA FEM code is developped under Linux (an Ubuntu 18.04 LTS is currently used for the development).

### Prerequisites:

Compilation of the FEM code requires a number of libraries.

Genaration of Makefiles for DynELA compilation is based on the use of the CMake application. CMAke is a cross-platform, open-source build system generator. It can be installed with the following command:

	sudo apt install cmake

DynELA is written in C++ and Python 3.x therefore it needs a C++ compiler and some Python 3.x libraries. Under Ubuntu this can be installed with the following command:

	sudo apt install build-essential swig zlib1g-dev liblapacke-dev python3-dev
	
It also needs some Python 3.x modules to run properly and at least numpy, matplotlib:

	sudo apt install python3-numpy python3-matplotlib texlive dvipng texlive-latex-extra texlive-fonts-recommended

### Download and compilation procedure:

Download and compilation is done using the following procedure:

	git clone https://github.com/pantale/DynELA.git
	cd DynELA
	mkdir Build
	cd Build
	cmake ../Sources
	make

There is no need to install. Modify the .bashrc file and add the following lines where _path_to_DynELA_ points to the top directory of your DynELA installation:

	export DYNELA="_path_to_DynELA_"
	export PATH=$PATH:$DYNELA/bin
	export DYNELA_BIN=$DYNELA/Build/bin
	export DYNELA_LIB=$DYNELA/Build/lib
	export PYTHONPATH="$DYNELA_BIN:$PYTHONPATH"
	export PYTHONPATH="$DYNELA_LIB:$PYTHONPATH"
	export LD_LIBRARY_PATH=$DYNELA_LIB:$LD_LIBRARY_PATH

## Testing and usage:

Running one of the provided samples in Samples floder:

	python sample.py

Running the tests in the Samples directory can be done through the Makefiles contained in the Samples directory. Benchmark tests can be run from any subdirectory of the Sample folder using the following command:

	make
	
## Postprocessing:

The DynELA FEM code can generate VTK files for the results. I'm using the Paraview postprocessor to visualize those results. Paraview is available here: https://www.paraview.org/download

***
Olivier Pantalé  
Full Professor of Mechanics  
email : olivier.pantale@enit.fr

Laboratoire Génie de Production  
Ecole Nationale d'Ingénieurs de Tarbes  
Université de Toulouse  
47 Avenue d'Azereix - BP 1629  
65016 TARBES - CEDEX - FRANCE