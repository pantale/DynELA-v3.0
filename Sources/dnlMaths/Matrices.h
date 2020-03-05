/***************************************************************************
 *                                                                         *
 *  DynELA Project                                                         *
 *                                                                         *
 *  (c) Copyright 1997-2004                                                *
 *                                                                         *
 *      Equipe C.M.A.O                                                     *
 *      Laboratoire Genie de production                                    *
 *      Ecole Nationale d'Ingenieurs de Tarbes                             *
 *      BP 1629 - 65016 TARBES cedex                                       *
 *                                                                         *
 *                                                                         *
 *  Main Author: Olivier PANTALE                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

// begin date :

/*
  Class Matrices definition
*/

#ifndef __dnlMaths_Matrices_h__
#define __dnlMaths_Matrices_h__
/*!
  \file Matrices.h
  \brief fichier .h de definition des matrices generales
  \ingroup linearAlgebra

  Ce fichier set e la definition de la classe Matrice qui sert de classe de base pour toutes les matrices de DynELA. Cette classe de base sert e stocker les informations communes e tous les types de matrices tels que la taille de la matrice (nombre de lignes et de colonnes)...

  \version 0.9.6
  \date 1997-2004
*/

#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <Errors.h>
#include <Macros.h>

#define Ind(i, j, __rows, __cols) ((i) * (__cols) + (j))

/*
  \enum OutMatrix
  \brief enumeration de format d'affichage de matrices.
  Cette enumeration permet de definir le format d'affichage des matrices. On a ainsi le choix entre outMatrixTypeNormal qui correspond e un affichage conforme e Mathematica et outMatrixTypeMupad qui correspond e un affichage au format Mupad.
  \since 1.0.0
*/
#ifndef SWIG
enum OutMatrix
{
  outMatrixTypeNormal = 0, // format Mathematica
  outMatrixTypeMupad       // format Mupad
};
#endif

/*!
  \class Matrices Matrices.h
  \brief classe de base pour les matrices
  \ingroup linearAlgebra

  Cette classe sert e la definition definition generale des matrices, elle sert de classe de base pour toutes les matrices de DynELA. Cette classe de base sert e stocker les informations communes e tous les types de matrices tels que la taille de la matrice (nombre de lignes et de colonnes)...

  \version 0.9.5
  \date 1997-2004
*/
class Matrices
{

protected:
  unsigned long _rows;      // nombre de lignes de la matrice
  unsigned long _cols;      // nombre de colonnes de la matrice
  char outType; // definit le type de format d'affichage

public:
  Matrices();
  Matrices(const Matrices &mat);
  virtual ~Matrices();

  long rows() const;
  long columns() const;
  void setOutType(char outT);
  virtual long Memory() const = 0; // calcule la taille requise pour le stockage de la matrice en memoire
};

//------inline functions-------------------------------------------------------

//constructeur par defaut
/*!
  Cette methode est le constructeur par defaut de la classe Matrices. Ce constructeur ne fait rien d'autre que creer l'objet.
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices()
//-----------------------------------------------------------------------------
{
  outType = outMatrixTypeNormal;
}

//constructeur par recopie
/*!
  Cette methode est le constructeur par recopie de la classe Matrices. Ce constructeur ne fait rien d'autre que creer l'objet.
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Matrices::Matrices(const Matrices &mat)
//-----------------------------------------------------------------------------
{
  outType = mat.outType;
}

//destructeur
/*!
  Cette methode est le destructeur de la classe Matrices.
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline Matrices::~Matrices()
//-----------------------------------------------------------------------------
{
}

//nombre de lignes de la matrice
/*!
  Cette methode calcule et retourne le nombre de lignes de la matrice.
  \nombre de lignes de la matrice
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline long Matrices::rows() const
//-----------------------------------------------------------------------------
{
  return _rows;
}

//nombre de colonnes de la matrice
/*!
  Cette methode calcule et retourne le nombre de colonnes de la matrice.
  \nombre de colonnes de la matrice
  \version 0.9.6
*/
//-----------------------------------------------------------------------------
inline long Matrices::columns() const
//-----------------------------------------------------------------------------
{
  return _cols;
}

//selection du type d'affichage
/*!
  Cette methode permet de selectionner le type d'affichage demande. Le type de sortie est defini par les variables enumerees dans \ref OutMatrix.

  \since 1.0.0
*/
//-----------------------------------------------------------------------------
inline void Matrices::setOutType(char outT)
//-----------------------------------------------------------------------------
{
  outType = outT;
}

#endif
