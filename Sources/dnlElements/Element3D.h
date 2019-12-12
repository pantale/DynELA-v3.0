/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
\file Element3D.h
\brief Declaration file for the Element3D class

This file is the declaration file for the Element3D class.

\ingroup dnlElements
\author &copy; Olivier PANTALE
\date 1997-2019
*/

#ifndef __dnlElements_Element3D_h__
#define __dnlElements_Element3D_h__

#include <Element.h>

class Element3D : public Element
{

public:
  // constructeurs
  Element3D(long elementNumber = 1);
  Element3D(const Element3D &X);
  ~Element3D();

  void getShapeFunctionAtPoint(Vector &shapeFunctions, const Vec3D &point) const = 0;
  void getDerShapeFunctionAtPoint(Matrix &derShapeFunctions, const Vec3D &point) const = 0;
  double getCharacteristicLength() = 0;
  double getRadiusAtIntegrationPoint();
  bool computeJacobian();
  void computeDeformationGradient(Tensor2 &F, short time);
  bool checkLevel2();
  void getV_atIntPoint(Vec3D &v, short time);
  void getdV_atIntPoint(Tensor2 &dv, short time);

  /*   long numberOfUnderIntegrationPoints () = 0;
  double getVolume () = 0;
  // bool getUnderIntegrPointCoords (long, Vec3D & coords, double& weight) = 0;
   //void getIntgtoNodes (Vector & N, const Vec3D & point) const = 0;
  void computeGlob2Loc () = 0;
  void glob2Loc (const Vec3D & point, Vec3D & local) = 0;

  void computeElasticStiffnessMatrix (Matrix & K, bool under=true);
  void getU_atIntPoint (Vec3D & u, short time);
    {
      
    }
  void getdU_atIntPoint (Tensor2 & du, short time);
  void computeDeformationGradient (Tensor2 & du, short time);
   */
};

#endif
