/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*
	Class Function implementation
*/
/*!
  \file Function.C
  \brief fichier .C de definition des fonctions discretes
  \ingroup linearAlgebra

  
  \author &copy; Olivier PANTALE
  \since 1.0.4
  \date 1997-2004
*/

#include <Function.h>

//!constructeur par defaut de la classe Function
/*!
  Ce constructeur alloue la memoire pour la creation d'une fonction discrete. Par defaut, la nouvelle fonction est definie avec le flag \ref Sort actif, c'est e dire que les points de la fonction sont automatiquement tries sur l'axe des abscisses.
  \author &copy; Olivier PANTALE
  \since 0.9.5
*/
//-----------------------------------------------------------------------------
Function::Function(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;
}

//-----------------------------------------------------------------------------
Function::~Function()
//-----------------------------------------------------------------------------
{
}
