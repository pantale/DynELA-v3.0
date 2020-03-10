/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
// TODOCXYFILE

// begin date : 05/03/1997

/*
  Class Tensor4 implementation
*/

#include <fstream>

#include <Tensor4.h>
#include <Tensor3.h>
#include <Tensor2.h>
#include <Vec3D.h>
#include <NumpyInterface.h>

/*!
  \file Tensor4.C
  \brief fichier .C de definition et de manipulation tenseurs d'ordre 4
  \ingroup dnlMaths

  Ce fichier definit les methodes permettant de gerer les tenseurs d'ordre 4.


  \since DynELA 0.9.5
  \date 1997-2004  
*/

//constructeur de la classe Tensor4
/*!
  Cette methode construit un tenseur d'ordre 4. Par defaut, le contenu du tenseur est mis à zero

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4::Tensor4()
//-----------------------------------------------------------------------------
{
  // initialisation
  setToValue(0.);
}

//destructeur de la classe Tensor4
//-----------------------------------------------------------------------------
Tensor4::~Tensor4()
//-----------------------------------------------------------------------------
{
}

//affichage du contenu d'un tenseur
/*!
  Cette methode est une surdefinition de << pour les flux de sortie, son utilisation est donnee comme suit
  
  Exemple
  \code
  Tensor4 t;
  std::cout << t << endl;
  \endcode
  \param os flux de sortie

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.print(os);
  return os;
}

//affichage du contenu d'un tenseur
/*!
  Cette methode permet d'afficher le contenu d'un tenseur sur la sortie std::ostream
  \param os flux de sortie

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
  long i, j, k, l;
  os << "tensor 3x3x3\n";
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      for (k = 0; k < 3; k++)
      {
        for (l = 0; l < 3; l++)
        {
          os << "T[" << i << "," << j << "," << k << "," << l << "]="
             << v[dnlTensor4Ind(i, j, k, l, 3)];
        }
      }
    }
  }
}

//renvoie un tenseur identite à partir d'un tenseur quelconque
/*!
  Cette methode prend en argument un tenseur quelconque et renvoie un tenseur identite
  
  Exemple :
  \code
  Tensor4 t1;
  t1.setToUnity(); // renvoie identite
  \endcode
  \warning Cette methode modifie son argument

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::setToUnity()
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          v[dnlTensor4Ind(i, j, k, l, 3)] =
              (dnlKronecker(i, k) * dnlKronecker(j, l) +
               dnlKronecker(i, l) * dnlKronecker(j, k)) /
              2.;
}

//affectation d'egalite
/*! 
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement le remplissage des valeurs d'un tenseur par un scalaire
  
  Exemple :
  \code
  Tensor4 t1;
  t1=1.; // affecte 1 à toutes les composantes du tenseur
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const double &val)
//-----------------------------------------------------------------------------
{
  setToValue(val);
  return *this;
}

//affectation d'egalite
/*! 
  Cette methode est une surdefinition de la methode d'egalite permettant d'ecrire simplement l'affectation sous la forme donnee en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2;
  t1=t2; // egalite de deux tenseurs
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4 &Tensor4::operator=(const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  memcpy(v, t1.v, 81 * sizeof(double));
  return *this;
}

//addition de deux tenseurs du deuxieme ordre
/*!
  Cette methode permet de surdefinir l'operation d'addition des tenseurs et d'ecrire simplement la somme de deux tenseurs sous la forme donnee en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2,t3;
  t3=t1+t2; // somme de deux tenseurs
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
Tensor4::operator+(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (long i = 0; i < 81; i++)
    t2.v[i] = v[i] + t1.v[i];

  // renvoi de l'objet
  return t2;
}

//soustraction de deux tenseurs du deuxieme ordre
/*!
  Cette methode permet de surdefinir l'operation de soustraction des tenseurs et d'ecrire simplement la soustraction de deux tenseurs sous la forme donnee en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2,t3;
  t3=t1-t2; // soustraction de deux tenseurs
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
Tensor4::operator-(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  // creation d'un nouveau tenseur
  Tensor4 t2;

  // calcul de la somme
  for (long i = 0; i < 81; i++)
    t2.v[i] = v[i] - t1.v[i];

  // renvoi de l'objet
  return t2;
}

//multiplication d'un tenseur par un scalaire
/*!
  Cette methode permet de surdefinir l'operation de multiplication des tenseurs et d'ecrire simplement la multiplication d'un tenseur par un scalaire sous la forme donnee en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2;
  double l;
  t2=t1*l; // multiplication par un scalaire
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
    Tensor4::operator*(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2.v[i] = lambda * v[i];

  return t2;
}

//division d'un tenseur par un scalaire
/*!
  Cette methode permet de surdefinir l'operation de division des tenseurs et d'ecrire simplement la division d'un tenseur par un scalaire sous la forme donnee en exemple
  
  Exemple :
  \code
  Tensor4 t1,t2;
  double l;
  t2=t1/l; // division par un scalaire
  \endcode
  \warning Cette operation n'est bien sur pas commutative

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
Tensor4::operator/(const double &lambda) const
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2.v[i] = v[i] / lambda;

  return t2;
}

//multiplication d'un tenseur par un scalaire
/*!
  Cette methode permet de surdefinir l'operation de multiplication des tenseurs et d'ecrire simplement la multiplication d'un tenseur par un scalaire sous la forme donnee en exemple. Elle est identique à la forme precedente (et commutative).
  
  Exemple :
  \code
  Tensor4 t1,t2;
  double l;
  t2=l*t1; // multiplication par un scalaire
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor4
operator*(const double &lambda, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  Tensor4 t2;

  for (long i = 0; i < 81; i++)
    t2.v[i] = lambda * t1.v[i];

  return t2;
}

//multiplication d'un tenseur par un vecteur
/*!
  Cette methode permet de surdefinir l'operation de multiplication des tenseurs et d'ecrire simplement la multiplication d'un tenseur par un vecteur sous la forme donnee en exemple. Cette operation correspond à la notion de produit contracte d'un tenseur et d'un vecteur, elle renvoie un vecteur \c Vec3D.
  
  Exemple :
  \code
  Tensor4 t1;
  Vec3D v1;
  Tensor3 t2;
  t2=t1*v1; // produit contracte
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor3 Tensor4::operator*(const Vec3D &v1) const
//-----------------------------------------------------------------------------
{
  Tensor3 t3;

  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          t3(i, j, k) += v[dnlTensor4Ind(i, j, k, l, 3)] * v1(l);

  return t3;
}

//multiplication de deux tenseurs
/*!
  Cette methode permet de surdefinir l'operation de multiplication des tenseurs et d'ecrire simplement la multiplication de deux tenseur sous la forme donnee en exemple. Cette operation correspond à la notion de double produit contracte de deux tenseurs.
  
  \param t1 deuxieme tenseur dans le double produit contracte
  
  Exemple :
  \code
  Tensor4 t1;
  Tensor2 t2,t3;
  t3=t1*t2; // double produit contracte
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
Tensor2
    Tensor4::operator*(const Tensor2 &t2) const
//-----------------------------------------------------------------------------
{
  Tensor2 t3;

  for (long i = 0; i < 3; i++)
    for (long j = 0; j < 3; j++)
      for (long k = 0; k < 3; k++)
        for (long l = 0; l < 3; l++)
          t3(i, j) += v[dnlTensor4Ind(i, j, k, l, 3)] * t2(k, l);

  return t3;
}

//egalite de deux tenseurs
/*!
  Cette methode teste l'egalite de deux tenseurs
  \return true si les deux tenseurs sont identiques, false dans la cas contraire

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
bool Tensor4::operator==(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  long i;

  for (i = 0; i < 81; i++)
    if (v[i] != t1.v[i])
      return false;
  return true;
}

//inegalite de deux tenseurs
/*!
  Cette methode teste l'inegalite de deux tenseurs
  \return true si les deux tenseurs sont differents, false dans la cas contraire

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
bool Tensor4::operator!=(const Tensor4 &t1) const
//-----------------------------------------------------------------------------
{
  return !(*this == t1);
}

//sortie sur flux std::ofstream
/*!
  Cette methode permet d'ecrire un tenseur dans un fichier (notament) binaire
  
  Exemple :
  \code
  std::ofstream pfile("fichier");
  Tensor4 t;
  t.write(pfile);
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::write(std::ofstream &ofs) const
//-----------------------------------------------------------------------------
{
  ofs.write((char *)v, 81 * sizeof(double));
}

//lecture sur flux std::ifstream
/*!
  Cette methode permet de lire un tenseur depuis un fichier (notament) binaire
  
  Exemple :
  \code
  std::ifstream pfile("fichier");
  Tensor4 t;
  t.read(pfile);
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
void Tensor4::read(std::ifstream &ifs)
//-----------------------------------------------------------------------------
{
  ifs.read((char *)v, 81 * sizeof(double));
}

//sortie sur flux std::ofstream
/*!
  Cette methode permet d'ecrire un tenseur dans un fichier (notament) binaire
  
  Exemple :
  \code
  std::ofstream pfile("fichier");
  Tensor4 t;
  pfile << t;
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
std::ofstream &operator<<(std::ofstream &os, const Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.write(os);
  return os;
}

//lecture sur flux std::ifstream
/*!
  Cette methode permet de lire un tenseur depuis un fichier (notament) binaire
  
  Exemple :
  \code
  std::ifstream pfile("fichier");
  Tensor4 t;
  pfile >> t;
  \endcode

  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
std::ifstream &operator>>(std::ifstream &is, Tensor4 &t1)
//-----------------------------------------------------------------------------
{
  t1.read(is);
  return is;
}

//Saves the content of a Tensor4 into a NumPy file
/*!
  This method saves the content of a Tensor4 object into a NumPy file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Tensor4 t;
  t.numpyWrite("numpy.npy", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyWrite(std::string filename, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npySave(filename, &v[0], {3, 3, 3, 3}, mode);
}

//Saves the content of a Tensor4 into a NumPyZ file
/*!
  This method saves the content of a vec3D object into a NumPyZ file defined by its filename. If the flag initialize is true, the current file will be concatenated.

  Example
  \code
  Tensor4 t;
  t.numpyWriteZ("numpy.npz", true);
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyWriteZ(std::string filename, std::string name, bool initialize) const
//-----------------------------------------------------------------------------
{
  std::string mode = "a";
  if (initialize)
    mode = "w";
  NumpyInterface::npzSave(filename, name, &v[0], {3, 3, 3, 3}, mode);
}

//Read the content of a Tensor4 from a NumPy file
/*!
  This method reads the content of a vec3D object from a NumPy file defined by its filename.

  Example
  \code
  Tensor4 t;
  t.numpyRead("numpy.npy");
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyRead(std::string filename)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npyLoad(filename);
  if (arr.num_vals != 81)
  {
    std::cout << "ERROR\n";
  }
  memcpy(v, arr.data<double *>(), arr.num_vals * arr.word_size);
}

//Read the content of a Tensor4 from a NumPyZ file
/*!
  This method reads the content of a vec3D object from a NumPyZ file defined by its filename.

  Example
  \code
  Tensor4 t;
  t.numpyReadZ("numpy.npz");
  \endcode
*/
//-----------------------------------------------------------------------------
void Tensor4::numpyReadZ(std::string filename, std::string name)
//-----------------------------------------------------------------------------
{
  NumpyInterface::NumpyArray arr = NumpyInterface::npzLoad(filename, name);
  if (arr.num_vals != 81)
  {
    std::cout << "ERROR\n";
  }
  memcpy(v, arr.data<double *>(), arr.num_vals * arr.word_size);
}