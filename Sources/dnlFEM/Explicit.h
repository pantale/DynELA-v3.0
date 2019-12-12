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

#ifndef __dnlFEM_Explicit_h__
#define __dnlFEM_Explicit_h__

#include <Solver.h>
//#include <Explicit.h>

/* #include <upFemSolver.h>

#include <Solver.h>
class Domain;
 */
/*!
  \file Explicit.h
  \brief fichier .h de definition des solveurs elements finis
  \ingroup femExplicitSolverCH

  Ce fichier sert e la definition de la classe Explicit.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/

#define defaultSpectralRadius 0.8182

/*!
  \class Explicit Explicit.h
  \brief Classe de definition et de manipulation des solveurs elements finis.
  \ingroup femExplicitSolverCH

  Cette classe sert e definir et e manipuler les solveurs disponibles au niveau d'un domaine. Elle contient toutes les definitions de base concernant la gestion des temps de simulation (increment de temps, temps de debut, de fin ...) ainsi que la gestion complete des increments.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
class Explicit : public Solver
{

private:
  short _timeStepMethod;
  double _rho_b;
  double _alphaM;
  double _alphaF;
  double _beta;
  double _gamma;

  void computeChungHulbertIntegrationParameters();

public:
  // constructor
  Explicit(char *newName = NULL);
  Explicit(const Explicit &X);
  ~Explicit();

  void setDissipation(double dissipation);
  void solve(double solveUpToTime);
  void initStep();
  void computePredictions();
  void explicitSolve();
  void endStep();
  void updateTime();
  void computeDensity();

};

#endif
