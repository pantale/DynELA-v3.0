/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*
	Class Function definition
*/
#ifndef __dnlMaths_Function_h__
#define __dnlMaths_Function_h__

#include <String.h>

/*!
  \file Function.h
  \brief fichier .h de definition des fonctions discretes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

/*!
  \class Function Function.h
  \brief Classe de definition des fonctions discretes
  \ingroup linearAlgebra

  \author &copy; Olivier PANTALE
  \date 1997-2004
  \since 1.0.4
*/
class Function
{
public:
  String name; //!< nom de la fonction discrete

public:
  Function(char* newName = NULL);
  virtual ~Function();

  virtual double getValue(double x) = 0;
  virtual double getSlope(double x) = 0;
#ifndef SWIG
  virtual void print() = 0;
#endif
  virtual String convertToDynELASourceFile() = 0;
};

#endif
