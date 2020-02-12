/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Field.h
  \brief Definition of the Mac Address class.

  This file defines the Mac Address class.
  \ingroup dnlKernel
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlKernel_Field_h__
#define __dnlKernel_Field_h__

#include <String.h>

#define FIELD_VEC3D(NAME) NAME, NAME##X, NAME##Y, NAME##Z
#define FIELD_TENSOR2(NAME) NAME, NAME##XX, NAME##XY, NAME##XZ, NAME##YX, NAME##YY, NAME##YZ, NAME##ZX, NAME##ZY, NAME##ZZ

class Field
{
public:
  static const char *const vtkNames[];

public:
  enum FieldLabel
  {
    FIELD_VEC3D(displacement),          // Displacement of a node
    FIELD_VEC3D(displacementIncrement), // Initial coordinates of a node increment
    FIELD_VEC3D(initialNodeCoordinate), // Initial coordinates of a node
    FIELD_VEC3D(nodeCoordinate),        // Coordinates of a node
    //FIELD_VEC3D(normal),                // Initial coordinates of a node
    FIELD_VEC3D(speed),              // Initial coordinates of a node
    FIELD_VEC3D(speedIncrement),     // Initial coordinates of a node increment
    density,                         // Material density
    plasticStrainRate,               // Equivalent plastic strain rate
    energy,                          // Internal energy
    energyIncrement,                 // Internal energy increment
    internalEnergy,                  // Internal energy
    plasticStrain,                   // Equivalent plastic deformation
    gamma,                           // gamma radial return value
    gammaCumulate,                   // Cumul of gamma values
    initialTemperature,              // Initial temperature
    mass,                            // Nodal Mass
    vonMises,                        // von Mises equivalent stress
    pressure,                        // Pressure
    temperature,                     // Current temperature
    timeStep,                        // Current timeStep
    realTimeStep,                    // Real timeStep taking into account the reducing due to saves
    kineticEnergy,                   // Kinetic energy of the current model
    yieldStress,                     // Maximum yield Stress of the point
    FIELD_TENSOR2(Strain),           // tenseur des deformations
    FIELD_TENSOR2(StrainInc),        // increment du tenseur des deformations
    FIELD_TENSOR2(PlasticStrain),    // tenseur des deformations plastiques
    FIELD_TENSOR2(PlasticStrainInc), // increment du tenseur des deformations plastiques
    FIELD_TENSOR2(DeviatoricStress), // Deviatoric part of the stress tensor \f$ s=\sigma-\frac{1}{3}tr[\sigma].I \f$
    FIELD_TENSOR2(Stress),           // Stress tensor
    ENDFIELDS
  };

public:
  Field();
  ~Field();

  short getField(String name);
  String getVtklabel(short field);
  short getType(short field);
};

#endif