/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
\file ElementPlane.h
\brief Declaration file for the ElementPlane class

This file is the declaration file for the ElementPlane class.

\ingroup dnlElements
\author &copy; Olivier PANTALE
\date 1997-2019
*/

#ifndef __dnlElements_ElTet10n3D_h__
#define __dnlElements_ElTet10n3D_h__

#include <Element3D.h>
class Element3D;

class ElTet10N3D : public Element3D
{

public:
  static const ElementData _elementData;

public:
  // constructeurs
  ElTet10N3D(long elementNumber = 1);
  ElTet10N3D(const ElTet10N3D &el);
  ~ElTet10N3D();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const  ;
  double getCharacteristicLength (){}

  /*  // fonction de manipulation
  long numberOfUnderIntegrationPoints ()
  {
    return 1;
  }
  
  // fonctions de calcul
  double getVolume();
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) ;
  void computeGlob2Loc();
  void glob2Loc(const Vec3D& point,Vec3D& local);
  //void getIntgtoNodes(Vector& N,const Vec3D& point) const; */
};

#endif
