/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
\file ElQua4NAx.h
\brief Declaration file for the ElQua4NAx class

This file is the declaration file for the ElQua4NAx class.

\ingroup dnlElements
\author &copy; Olivier PANTALE
\date 1997-2019
*/

#ifndef __dnlElements_ElQua4nAx_h__
#define __dnlElements_ElQua4nAx_h__


class ElementAxi;
#include <ElementAxi.h>

class ElQua4NAx : public ElementAxi
{

public:
  static const ElementData _elementData;

public:
  ElQua4NAx(long elementNumber = 1);
  ElQua4NAx(const ElQua4NAx &element);
  ~ElQua4NAx();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const;
  double getCharacteristicLength();
  double getArea();
};

#endif
