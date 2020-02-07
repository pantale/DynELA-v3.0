/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file CrackFunction.h
  \brief Declaration file for the discrete function class

  This file is the declaration file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2019
*/

#ifndef __dnlMaths_CrackFunction_h__
#define __dnlMaths_CrackFunction_h__

#include <List.h>
#include <DiscreteFunction.h>

/*!
  \class CrackFunction
  \brief This special type of discrete function is used to define cracks in an X-FEM formulation.
  \ingroup linearAlgebra
  \author &copy; Olivier PANTALE
  \since DynELA 1.0
  \date 1997-2019
*/
class CrackFunction : public DiscreteFunction
{

public:
    CrackFunction ();
    CrackFunction ( const CrackFunction &X );
    ~CrackFunction ();

    void headTipAdd ( double x, double y );
    void tailTipAdd ( double x, double y );
    bool isOnPositiveSide ( double x, double y );
    bool distanceToSegmentShort ( double &dist,long i, double x, double y );
};

#endif

