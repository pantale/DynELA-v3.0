#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 09:43:44 2018

@author: pantale
"""

import dnlPython as dnl

nbreSaves = 10      # nombre de fichiers graphiques
nbrePoints = 100    # nombre de points dans le time history
stopTime = 0.01      # temps total de simulation
displacement = 100   # déplacement total en cisaillement

# Material parameters
young = 206000
poisson = 0.3
density = 7.83e-09
heatCapacity = 4.6e+08
taylorQuinney = 0.9
A = 806.0
B = 614.0
C = 0.0089
n = 0.168
m = 1.1
depsp0 = 1.0
Tm = 1540.0
T0 = 20.0

# Creates the main Object
dynELA = dnl.DynELA("Shear")
domain = dynELA.getCurrentDomain()

# Creates the Nodes
dynELA.createNode(1, 0.00, 0.00, 0.00)
dynELA.createNode(2, 10.0, 0.00, 0.00)
dynELA.createNode(3, 10.0, 10.0, 0.00)
dynELA.createNode(4, 0.00, 10.0, 0.00)
print("Number of nodes created:", dynELA.getNodesNumber())    

# Creates the Elements
dynELA.setDefaultElement(dnl.Element.ElQua4N2D)
dynELA.createElement(1, 1, 2, 3, 4)
print("Number of elements created:", dynELA.getElementsNumber())    

# Creates the Groups
allNS = dnl.NodeSet("NS_All")
dynELA.add(allNS, 1, 4)

topNS = dnl.NodeSet("NS_Top")
dynELA.add(topNS, 3, 4)

bottomNS = dnl.NodeSet("NS_Bottom")
dynELA.add(bottomNS, 1, 2)

allES = dnl.ElementSet("ES_All")
dynELA.add(allES, 1)

histNS = dnl.NodeSet("NS_Hist")
dynELA.add(histNS, 1)

histES = dnl.NodeSet("ES_Hist")
dynELA.add(histES, 1)

# Creates the hardening law
hardLaw = dnl.JohnsonCookLaw()
hardLaw.setParameters(A, B, C, n, m, depsp0, Tm, T0)

# Creates the material
steel = dnl.Material("Steel")
steel.setHardeningLaw(hardLaw)
steel.youngModulus = young
steel.poissonRatio = poisson
steel.density = density
steel.heatCapacity = heatCapacity
steel.taylorQuinney = taylorQuinney
steel.initialTemperature = T0

# Finaly link the material to the structure
dynELA.add(steel, allES)

# Declaration of a boundary condition for bottom part
bottomBC = dnl.BoundaryRestrain('BC_bottom')
bottomBC.setValue(1, 1, 1)
dynELA.attachConstantBC(bottomBC, bottomNS)

# Declaration of a boundary condition for top part
topBC = dnl.BoundaryRestrain('BC_top')
topBC.setValue(0, 1, 1)
dynELA.attachConstantBC(topBC, topNS)

# Declaration of a ramp function to apply the load
ramp = dnl.RampFunction("constantFunction")
ramp.set(dnl.RampFunction.Constant, 0, stopTime)

# Declaration of a boundary condition for top part
topSpeed = dnl.BoundarySpeed()
topSpeed.setValue(displacement, 0, 0)
topSpeed.setFunction(ramp)
dynELA.attachConstantBC(topSpeed, topNS)

# Declaration of the explicit solver
solver = dnl.Explicit("Solver")
solver.setTimes(0, stopTime)
domain.add(solver)
dynELA.setSaveTimes(0, stopTime, stopTime/nbreSaves)

# Declaration of the history files
vonMisesHist = dnl.HistoryFile("vonMisesHistory")
vonMisesHist.setFileName(dnl.String("vonMises.plot"))
vonMisesHist.add(histNS, dnl.Field.vonMises)
vonMisesHist.setSaveTime(stopTime/nbrePoints)
domain.add(vonMisesHist)

StressHist = dnl.HistoryFile("StressHistory")
StressHist.setFileName(dnl.String("Stress.plot"))
StressHist.add(histNS, dnl.Field.StressXX)
StressHist.add(histNS, dnl.Field.StressYY)
StressHist.add(histNS, dnl.Field.StressZZ)
StressHist.add(histNS, dnl.Field.StressXY)
StressHist.setSaveTime(stopTime/nbrePoints)
domain.add(StressHist)

temperatureHist = dnl.HistoryFile("temperatureHistory")
temperatureHist.setFileName(dnl.String("temperature.plot"))
temperatureHist.add(histNS, dnl.Field.temperature)
temperatureHist.setSaveTime(stopTime/nbrePoints)
domain.add(temperatureHist)

plasticStrainHist = dnl.HistoryFile("plasticStrainHistory")
plasticStrainHist.setFileName(dnl.String("plasticStrain.plot"))
plasticStrainHist.add(histNS, dnl.Field.plasticStrain)
plasticStrainHist.setSaveTime(stopTime/nbrePoints)
domain.add(plasticStrainHist)

densityHist = dnl.HistoryFile("densityHistory")
densityHist.setFileName(dnl.String("density.plot"))
densityHist.add(histNS, dnl.Field.density)
densityHist.setSaveTime(stopTime/nbrePoints)
domain.add(densityHist)

dtHist = dnl.HistoryFile("dtHistory")
dtHist.setFileName(dnl.String("dt.plot"))
dtHist.add(dnl.Field.timeStep)
dtHist.setSaveTime(stopTime/nbrePoints)
domain.add(dtHist)

keHist = dnl.HistoryFile("keHistory")
keHist.setFileName(dnl.String("ke.plot"))
keHist.add(dnl.Field.kineticEnergy)
keHist.setSaveTime(stopTime/nbrePoints)
domain.add(keHist)

# Parallel solver
parallel = dnl.Parallel()
dynELA.add(parallel)
parallel.setCores(1)

# Run the main solver
dynELA.solve()

# Plot the results as curves
import dnlCurves as cu
curves = cu.Curves()
curves.plotFile('Curves.ex')
