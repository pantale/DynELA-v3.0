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

// begin date : 05/03/1997

/*
  Class Tensor4 definition
*/

#ifndef __dnlMaths_Tensor4_h__
#define __dnlMaths_Tensor4_h__

/*!
  \file Tensor4.h
  \brief fichier .h de definition et de manipulation tenseurs d'ordre 4
  \ingroup linearAlgebra

  Ce fichier definit les methodes permettant de gerer les tenseurs d'ordre 4.

  \author Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004  
*/

#include <Vec3D.h>
#include <Tensor2.h>
#include <Tensor3.h>

class Vec3D;

/*!
  \class Tensor4 Tensor4.h
  \brief Classe de gestion et manipulation des tenseurs du second ordre.
  \ingroup linearAlgebra
  \author Olivier PANTALE
  \version 0.9.5
  \date 1997-2004

Cette classe est utilisee pour la gestion et la manipulation des tenseurs du quatrieme ordre. Elle comporte toutes les methodes d'initialisation et de gestion memoire. Les valeurs stockees sont des double. La classe \c Tensor4 contient de plus un certain nombre de methodes de calcul sur les tenseurs comme decrit ci-dessous.
*/
/** @dia:pos 64,78 */
class Tensor4
{
  double v[81];

public:
  // constructeurs
  Tensor4();
  ~Tensor4();

  // operations sur les composantes
  inline double operator()(long i, long j, long k, long l) const;
#ifndef SWIG
  inline double &operator()(long i, long j, long k, long l);
#endif
  inline bool indexOK(long i, long j, long k, long l) const;

  // operations d'affectation
  inline void setToValue(double val);
  void setToUnity();
#ifndef SWIG
  Tensor4 &operator=(const double &val);
  Tensor4 &operator=(const Tensor4 &tens);
#endif

  // operations de comparaison
  bool operator==(const Tensor4 &tens) const;
  bool operator!=(const Tensor4 &tens) const;

  // operations arithmetiques de base entre tenseurs
#ifndef SWIG
  inline void operator+=(const Tensor4 &tens);
  inline void operator-=(const Tensor4 &tens);
  inline void operator*=(const double val);
  inline void operator/=(const double val);
#endif
  Tensor4 operator+(const Tensor4 &tens) const;
  Tensor4 operator-(const Tensor4 &tens) const;
  Tensor4 operator*(const double &lambda) const;
  Tensor4 operator/(const double &tens) const;
#ifndef SWIG
  friend Tensor4 operator*(const double &val, const Tensor4 &tens);
#endif

  // multiplications particulieres
  Tensor3 operator*(const Vec3D &vec) const;
  Tensor2 operator*(const Tensor2 &t) const;
  void numpyWrite(std::string filename, bool initialize = false) const;
  void numpyWriteZ(std::string filename, std::string name, bool initialize = false) const;
  void numpyRead(std::string filename);
  void numpyReadZ(std::string filename, std::string name);

  // gestion des flux entree et sortie
#ifndef SWIG
  friend std::ostream &operator<<(std::ostream &os, const Tensor4 &tens);
  friend std::ofstream &operator<<(std::ofstream &os, const Tensor4 &tens);
  friend std::ifstream &operator>>(std::ifstream &is, Tensor4 &tens);
  void write(std::ofstream &ofs) const;
  void read(std::ifstream &ifs);
  void print(std::ostream &os) const;
#endif
};

//------inline functions-------------------------------------------------------

//!teste les bornes du tenseur
/*!
  Cette methode teste les bornes d'un tenseur
  \param i long 1
  \param j long 2
  \param k long 3
  \param l long 4
  \return true si les indices fournis sont dans les bornes, false dans le cas contraire
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline bool
Tensor4::indexOK(long i, long j, long k, long l) const
//-----------------------------------------------------------------------------
{
  if ((i >= 0) && (i < 3) && (j >= 0) && (j < 3) && (k >= 0) && (k < 3) && (l >= 0) && (l < 3))
    return (true);

  printf("Tensor4::indexOK\nindice(s) [%ld,%ld,%ld,%ld] out of allowed range [0-2,0-2,0-2,0-2]",
         i, j, k, l);
  exit(-1);

  // fonction inutile, mais necessaire pour le compilateur
  return (false);
}

//!acces aux valeurs d'un tenseur
/*!
  \param i long 1
  \param j long 2
  \param k long 3
  \param l long 4
  \return valeur du tenseur Tijkl
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline double &
Tensor4::operator()(long i, long j, long k, long l)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j, k, l);
#endif
  return v[dnlTensor4Ind(i, j, k, l, 3)];
}

//!acces aux valeurs d'un tenseur
/*!
  \param i long 1
  \param j long 2
  \param k long 3
  \param l long 4
  \return valeur du tenseur Tijkl
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline double
Tensor4::operator()(long i, long j, long k, long l)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_math
  indexOK(i, j, k, l);
#endif
  return v[dnlTensor4Ind(i, j, k, l, 3)];
}

//!affectation d'egalite
/*! 
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement le remplissage des valeurs d'un tenseur par un scalaire

  Exemple :
  \code
  Tensor4 t1;
  t1=setToValue(1.); // affecte 1 e toutes les composantes du tenseur
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void Tensor4::setToValue(double val)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < 81; i++)
    v[i] = val;
}

//!addition de deux tenseurs du deuxieme ordre
/*!
  Cette methode permet de surdefinir l'operation d'addition des tenseurs et d'ecrire simplement la somme de deux tenseurs sous la forme donnee en exemple

  Exemple :
  \code
  Tensor4 t1,t2;
  t2+=t1; // somme de deux tenseurs
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator+=(const Tensor4 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la somme
  for (long i = 0; i < 81; i++)
    v[i] += tens.v[i];
}

//!soustraction de deux tenseurs du deuxieme ordre
/*!
  Cette methode permet de surdefinir l'operation de soustraction des tenseurs et d'ecrire simplement la soustraction de deux tenseurs sous la forme donnee en exemple

  Exemple :
  \code
  Tensor4 t1,t2;
  t2-=t1; // soustraction de deux tenseurs
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator-=(const Tensor4 &tens)
//-----------------------------------------------------------------------------
{
  // calcul de la difference
  for (long i = 0; i < 81; i++)
    v[i] -= tens.v[i];
}

//!multiplication d'un tenseur par un scalaire
/*!
  Cette methode permet de surdefinir l'operation de multiplication des tenseurs et d'ecrire simplement la multiplication d'un tenseur par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  Tensor4 t1;
  double l;
  t1*=l; // multiplication par un scalaire
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator*=(const double val)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < 81; i++)
    v[i] *= val;
}

//!division d'un tenseur par un scalaire
/*!
  Cette methode permet de surdefinir l'operation de division des tenseurs et d'ecrire simplement la division d'un tenseur par un scalaire sous la forme donnee en exemple

  Exemple :
  \code
  Tensor4 t1;
  double l;
  t1/=l; // division par un scalaire
  \endcode
  \author Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
inline void
Tensor4::operator/=(const double val)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < 81; i++)
    v[i] /= val;
}

#endif
