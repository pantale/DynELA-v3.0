/***************************************************************************
*                                                                         *
*  DynELA Finite Element Code v 3.0                                       *
*  By Olivier PANTALE                                                     *
*                                                                         *
*  (c) Copyright 1997-2019                                                *
*                                                                         *
**************************************************************************/

/*!
\file Error.C
\brief Definition of the errors for the DynELA FEM code

This file defines the errors and warning that are used in the DynELA FEM code.
The functions defined in this files serves to display messages and warnings.
\ingroup basicTools
\author &copy; Olivier PANTALE
\since DynELA 1.0
\date 1997-2019
*/

#include <Field.h>
#include <Errors.h>

#define NAME_VEC3D(NAME) NAME, NAME "X", NAME "Y", NAME "Z"
#define NAME_TENSOR2(NAME) NAME, NAME "XX", NAME "XY", NAME "XZ", NAME "YX", NAME "YY", NAME "YZ", NAME "ZX", NAME "ZY", NAME "ZZ"

const char *const Field::vtkNames[] = {
    NAME_VEC3D("displacement"),
    NAME_VEC3D("displacementIncrement"),
    NAME_VEC3D("initialNodeCoordinate"),
    NAME_VEC3D("nodeCoordinate"),
    NAME_VEC3D("normal"),
    NAME_VEC3D("speed"),
    NAME_VEC3D("speedIncrement"),
    "density",
    "plasticStrainRate",
    "energy",
    "energyIncrement",
    "internalEnergy",
    "plasticStrain",
    "gamma",
    "gammaCumulate",
    "initialTemperature",
    "mass",
    "vonMises",
    "pressure",
    "temperature",
    "timeStep",
    "realTimeStep",
    "kineticEnergy",
    "yield",
    NAME_TENSOR2("Strain"),
    NAME_TENSOR2("StrainIncrement"),
    NAME_TENSOR2("PlasticStrain"),
    NAME_TENSOR2("PlasticStrainIncrement"),
    NAME_TENSOR2("DeviatoricStress"),
    NAME_TENSOR2("Stress"), "ENDFIELDS"};

//-----------------------------------------------------------------------------
Field::Field()
//-----------------------------------------------------------------------------
{
  if (vtkNames[ENDFIELDS] != "ENDFIELDS")
    fatalError("Field::Field", "Lists of fields doesn't contain the same number of elements");
}

//-----------------------------------------------------------------------------
Field::~Field()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
String Field::getVtklabel(short field)
//-----------------------------------------------------------------------------
{
  return vtkNames[field];
}

//-----------------------------------------------------------------------------
short Field::getField(String name)
//-----------------------------------------------------------------------------
{
  short fieldId = 0;
  while (fieldId != ENDFIELDS)
  {
    if (vtkNames[fieldId] == name)
      return fieldId;
    fieldId += 1;
  }
  return -1;
}

//-----------------------------------------------------------------------------
short Field::getType(short field)
//-----------------------------------------------------------------------------
{
  if (field == Field::displacement)
    return 1;
  if (field == Field::displacementIncrement)
    return 1;
  if (field == Field::initialNodeCoordinate)
    return 1;
  if (field == Field::nodeCoordinate)
    return 1;
  if (field == Field::normal)
    return 1;
  if (field == Field::speed)
    return 1;
  if (field == Field::speedIncrement)
    return 1;
  if (field == Field::Strain)
    return 2;
  if (field == Field::StrainInc)
    return 2;
  if (field == Field::PlasticStrain)
    return 2;
  if (field == Field::PlasticStrainInc)
    return 2;
  if (field == Field::DeviatoricStress)
    return 2;
  if (field == Field::Stress)
    return 2;

  return 0;
}