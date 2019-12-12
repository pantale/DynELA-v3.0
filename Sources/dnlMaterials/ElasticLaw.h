/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file ElasticLaw.h
  \brief Declaration file for the ElasticLaw class

  This file is the declaration file for the ElasticLaw class.

  \ingroup dnlMaterials
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlMaterial_ElasticLaw_h__
#define __dnlMaterial_ElasticLaw_h__

#include <HardeningLaw.h>

class ElasticLaw : public HardeningLaw
{

public:
  ElasticLaw();
  ElasticLaw(const ElasticLaw &);
  ~ElasticLaw();

  int getNumberOfParameters();
  const char *getParameterName(int parameter);
  double &getParameter(int parameter);
  double getYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
  double getDerivateYieldStress(double plasticStrain, double plasticStrainRate = 0.0, double T = 0.0, double dtime = 0.0, IntegrationPoint *intPoint = NULL);
};

#endif
