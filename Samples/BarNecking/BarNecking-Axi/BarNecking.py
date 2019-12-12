#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 09:43:44 2018

@author: pantale
"""

import dnlPython as dnl

# Parameters of the model
nbreSaves = 20
stopTime = 1e-3
nbrePoints = 1000 
bottomWidth = 6.35
topWidth = 6.413
heigh = 26.67
nbElementsWidth = 10
nbElementsHeigh = 40
factor = 1 / 2
displacement = 7

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

dxWidth = bottomWidth / nbElementsWidth
dxHeigh = heigh / nbElementsHeigh
ddx = (topWidth - bottomWidth) / bottomWidth / heigh
speed = displacement / stopTime

# Creates the main Object
dynELA = dnl.DynELA("BarNecking")
domain = dynELA.getCurrentDomain()

# Creates the Nodes
nbNodes = 1
nsAll = dnl.NodeSet("NS_All")
dxHeigh /=  (1 / factor + 1) / 2
y = 0
for j in range (nbElementsHeigh + 1): 
    for i  in range (nbElementsWidth + 1):
        dynELA.createNode(nbNodes, i * dxWidth * (1 + ddx * y), y, 0)
        dynELA.add(nsAll, nbNodes)
        nbNodes += 1
    if (j == nbElementsHeigh/2): dxHeigh /= factor
    y +=  dxHeigh
nbNodes -= 1
print("Number of nodes created:", dynELA.getNodesNumber())    

# Creates the Elements
dynELA.setDefaultElement(dnl.Element.ElQua4NAx)
nbElements = 1
esAll = dnl.ElementSet("ES_All")
for j in range (nbElementsHeigh):
    for i in range (nbElementsWidth):
        n1 = (i + (j * (nbElementsWidth + 1)) + 1)
        n2 = (i + (j * (nbElementsWidth + 1)) + 2)
        n3 = (i + ((j + 1) * (nbElementsWidth + 1)) + 2)
        n4 = (i + ((j + 1) * (nbElementsWidth + 1)) + 1)
        dynELA.createElement(nbElements, n1, n2, n3, n4)
        dynELA.add(esAll, nbElements)
        nbElements += 1
nbElements -= 1
print("Number of elements created:", dynELA.getElementsNumber())    

topNS = dnl.NodeSet("NS_Top")
dynELA.add(topNS, nbNodes-nbElementsWidth, nbNodes)

bottomNS = dnl.NodeSet("NS_Bottom")
dynELA.add(bottomNS, 1, nbElementsWidth + 1)

axisNS = dnl.NodeSet("NS_Axis")
dynELA.add(axisNS, 1, nbNodes, nbElementsWidth + 1)

histNS = dnl.NodeSet("NS_Hist")
dynELA.add(histNS, 1)

histES = dnl.ElementSet("ES_Hist")
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
dynELA.add(steel, esAll)

# Declaration of a boundary condition for bottom line
bottomBC = dnl.BoundaryRestrain('BC_bottom')
bottomBC.setValue(0, 1, 1)
dynELA.attachConstantBC(bottomBC, bottomNS)

# Declaration of a boundary condition for axis line
axisBC = dnl.BoundaryRestrain('BC_axis')
axisBC.setValue(1, 0, 1)
dynELA.attachConstantBC(axisBC, axisNS)

# Declaration of a boundary condition for top line
speedBC = dnl.BoundarySpeed('BC_speed')
speedBC.setValue(0, speed, 0)
dynELA.attachConstantBC(speedBC, topNS)

solver = dnl.Explicit("Solver")
solver.setTimes(0, stopTime)
domain.add(solver)
#solver.setTimeStepSafetyFactor(0.6)
dynELA.setSaveTimes(0, stopTime, stopTime / nbreSaves)

# Declaration of the history files
vonMisesHist = dnl.HistoryFile("vonMisesHistory")
vonMisesHist.setFileName(dnl.String("vonMises.plot"))
vonMisesHist.add(histNS, dnl.Field.vonMises)
vonMisesHist.setSaveTime(stopTime / nbrePoints)
domain.add(vonMisesHist)

plasticStrainHist = dnl.HistoryFile("plasticStrainHistory")
plasticStrainHist.setFileName(dnl.String("plasticStrain.plot"))
plasticStrainHist.add(histNS, dnl.Field.plasticStrain)
plasticStrainHist.setSaveTime(stopTime / nbrePoints)
domain.add(plasticStrainHist)

temperatureHist = dnl.HistoryFile("temperatureHistory")
temperatureHist.setFileName(dnl.String("temperature.plot"))
temperatureHist.add(histNS, dnl.Field.temperature)
temperatureHist.setSaveTime(stopTime / nbrePoints)
domain.add(temperatureHist)

internalEnergyHist = dnl.HistoryFile("internalEnergyHistory")
internalEnergyHist.setFileName(dnl.String("internalEnergy.plot"))
internalEnergyHist.add(histNS, dnl.Field.internalEnergy)
internalEnergyHist.setSaveTime(stopTime / nbrePoints)
domain.add(internalEnergyHist)

dtHist = dnl.HistoryFile("dtHistory")
dtHist.setFileName(dnl.String("dt.plot"))
dtHist.add(dnl.Field.timeStep)
dtHist.setSaveTime(stopTime / nbrePoints)
domain.add(dtHist)

keHist = dnl.HistoryFile("keHistory")
keHist.setFileName(dnl.String("ke.plot"))
keHist.add(dnl.Field.kineticEnergy)
keHist.setSaveTime(stopTime / nbrePoints)
domain.add(keHist)

gammaHist = dnl.HistoryFile("gammaHistory")
gammaHist.setFileName(dnl.String("gamma.plot"))
gammaHist.add(histES, 1, dnl.Field.gamma)
gammaHist.add(histES, 2, dnl.Field.gamma)
gammaHist.add(histES, 3, dnl.Field.gamma)
gammaHist.add(histES, 4, dnl.Field.gamma)
gammaHist.setSaveTime(stopTime / nbrePoints)
domain.add(gammaHist)

gammaCumulateHist = dnl.HistoryFile("gammaCumulateHistory")
gammaCumulateHist.setFileName(dnl.String("gammaCumulate.plot"))
gammaCumulateHist.add(histES, 1, dnl.Field.gammaCumulate)
gammaCumulateHist.add(histES, 2, dnl.Field.gammaCumulate)
gammaCumulateHist.add(histES, 3, dnl.Field.gammaCumulate)
gammaCumulateHist.add(histES, 4, dnl.Field.gammaCumulate)
gammaCumulateHist.setSaveTime(stopTime / nbrePoints)
domain.add(gammaCumulateHist)

# Declaration of the history files
vonMisesHist2 = dnl.HistoryFile("vonMisesHistory2")
vonMisesHist2.setFileName(dnl.String("vonMises2.plot"))
vonMisesHist2.add(histES, 1, dnl.Field.vonMises)
vonMisesHist2.add(histES, 2, dnl.Field.vonMises)
vonMisesHist2.add(histES, 3, dnl.Field.vonMises)
vonMisesHist2.add(histES, 4, dnl.Field.vonMises)
vonMisesHist2.setSaveTime(stopTime / nbrePoints)
domain.add(vonMisesHist2)

parallel = dnl.Parallel()
dynELA.add(parallel)
parallel.setCores(4)

dynELA.solve()

# Plot the results as curves
import dnlCurves as cu
curves = cu.Curves()
curves.plotFile('Curves.ex')
