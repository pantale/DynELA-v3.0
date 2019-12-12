/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlFEM_ExplicitOld_h__
#define __dnlFEM_ExplicitOld_h__

#include <Solver.h>

class ExplicitOld : public Solver
{

private:
  short _timeStepMethod;

public:
  // constructor
  ExplicitOld(char *newName = NULL);
  ExplicitOld(const ExplicitOld &X);
  ~ExplicitOld();

  void solve(double solveUpToTime);
  void computePredictions();
  void updateGrid();
  void computeStrains();
  void computeConstitutive();
  void computeState ();
  void computeStress ();
  void computeMass ();
  void computeMomentum ();
  void computeCorrections();
  void updateTime ();

  // member functions
/*   
  void computeForces ();
  void computeEnergy ();
  void computeTemperatures ();
  void scanInterfaces ();
  void computeContactForces ();
  const char* Name() {return "ExplicitOld";}
 */};

#endif
