/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file DiscreteFunctionSet.h
  \brief Declaration file for the discrete function class

  This file is the declaration file for the discrete function class. A discrete function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_DiscreteFunctionSet_h__
#define __dnlMaths_DiscreteFunctionSet_h__

#include <DiscreteFunction.h>

/*!
  \brief This is a set of discrete functions.
  
  This class is used to manage sets of discrete functions and group them. This is a usefull class for the \b DynELA \b curves application.
  \ingroup dnlMaths
*/
class DiscreteFunctionSet
{
    List <DiscreteFunction *> curves;

public:
    DiscreteFunctionSet ();
    DiscreteFunctionSet ( const DiscreteFunctionSet &X );
    ~DiscreteFunctionSet ();

    void read ( String );
    void write ( String );
    DiscreteFunction *get ( String );
    DiscreteFunction *get ( long );
    void add ( DiscreteFunction * );
    void remove ( long );
    void remove ( String );
    long getSize();
};

#endif

