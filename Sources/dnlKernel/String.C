/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

#include "String.h"
#include <strings.h>
#include <iostream>

/*!
  \file String.C
  \brief fichier . h de definition des chaines de caracteres
  \ingroup basicTools

  Ce fichier regroupe la definition des chaines de caracteres pour DynELA. Les chaines de caracteres font partie d'une nouvelle classe nommee String qui permet la pluspart des manipulations sur les caracteres. L'ensemble des methodes de cette classe sont documentees.

  Depuis la version v. 1.0.0, cette classe herite de la classe \b std::string de Standard Template Library.

  \author &copy; Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/

//!constructeur par defaut de la classe String
/*!
  Allocation memoire pour une chaine de caracteres vide. Cette methode est utilisee pour creer l'allocation memoire pour une chaine de caracteres vide.

  Exemple:
  \code
  String s; // allocation memoire vide (taille indefinie)
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String::String()
//-----------------------------------------------------------------------------
{
}

//!constructeur de la classe String e partir d'un caractere unique
/*!
  Ce constructeur alloue l'espace memoire pour une chaine de caracteres e partir de la donnee d'un seul caractere. Cette methode peut sembler inutile car utiliser un String pour un seul caractere est bien bete, mais elle permet de faire l'addition de petits bouts de chaine de caracteres.

  Exemple:
  \code
  String s = 'k'; // s est une chaine d'un caractere 'k'
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
// //-----------------------------------------------------------------------------
// String::String (const char c) : std::string (1, c)
// //-----------------------------------------------------------------------------
// {
// }

//!constructeur e partir d'un type char*
/*!
  Ce constructeur permet d'allouer l'espace memoire pour un String et d'initialiser cette chaine de caracteres e partir d'une suite de caracteres standerd de type char*.
  
  Exemple:
  \code
  String y = "a std::string";
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String::String(const char *s) // : std::string (s)
//-----------------------------------------------------------------------------
{
  assign(s);
}

//-----------------------------------------------------------------------------
String::String(const std::string &st) // : std::string (st)
//-----------------------------------------------------------------------------
{
  assign(st);
}

//-----------------------------------------------------------------------------
String::String(const String &st) // : std::string (st)
//-----------------------------------------------------------------------------
{
  assign(st);
}
//!destructeur associe e la classe String
/*!
  Ceci est le destructeur de la classe String.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String::~String()
//-----------------------------------------------------------------------------
{
}

//!constructeur e partir d'un type std::string
/*!
  Ce constructeur permet d'allouer l'espace memoire pour un String et d'initialiser cette chaine de caracteres e partir d'une suite de caracteres standerd de type char*.
  
  Exemple:
  \code
  String y = "a std::string";
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::operator=(const char *st)
//-----------------------------------------------------------------------------
{
  assign(st);
  return (*this);
}

// String::operator const char *() const
// {
//   return c_str();
// }
// //-----------------------------------------------------------------------------
// String & String::operator = (const char* st)
// //-----------------------------------------------------------------------------
// {
// (*this)=st;

//  return (*this);
// }

//!conversion d'une valeur numerique entiere en un String
/*!
  Cette methode permet de convertir une valeur numerique de type int en une chaine de caracteres.

  Exemple:
  \code
  String toto;
  toto.convert(2);
  cout << toto << endl;
  \endcode
  \param val valeur numerique e convertir en un String
  \return La chaine de caracteres de retour.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::convert(int val, short leading)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, "%d", val);
  assign(tmpStr);
  if (length() < leading)
  {
    String leadString = std::string(leading - length(), '0');
    *this = leadString + *this;
  }
  return *this;
}

//!conversion d'une valeur numerique entiere en un String
/*!
  Cette methode permet de convertir une valeur numerique de type int en une chaine de caracteres.

  Exemple:
  \code
  String toto;
  toto.convert(2);
  cout << toto << endl;
  \endcode
  \param val valeur numerique e convertir en un String
  \return La chaine de caracteres de retour.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::convert(long val, short leading)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, "%ld", val);
  assign(tmpStr);
  if (length() < leading)
  {
    String leadString = std::string(leading - length(), '0');
    *this = leadString + *this;
  }
  return *this;
}

//!conversion d'une valeur reelle en un String
/*!
  Cette methode permet de convertir une valeur numerique reelle en une chaine de caracteres en specifiant le type de conversion par une chaine de caracteres de contrele.

  Exemple:
  \code
  String toto;
  toto.convert(2.0,"%10.3E");
  cout << toto << endl;
  \endcode
  \param val valeur reelle e convertir en un String
  \param st format de sortie de la chaine de caracteres. par defaut, si on ne precise rien, on utilise un format de type "%ld"
  \return chaine de caracteres de retour
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::convert(double val, const char *st)
//-----------------------------------------------------------------------------
{
  char tmpStr[100];
  sprintf(tmpStr, st, val);
  assign(tmpStr);
  return *this;
}

//!remplacement d'un caractere dans une chaine
/*!
  Cette methode recherche et remplace un caractere donne par un autre dans une chaine de caracteres.
  \param fromItem caractere e remplacer dans la chaine
  \param to caractere de remplacement dans la chaine
  \return nouvelle chaine de caracteres avec la substitution
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::replace(const char fromItem, const char to)
//-----------------------------------------------------------------------------
{
  std::string _to(1, to);
  const char *p = data();
  int l = length();

  // boucle de recherche
  for (int i = 0; i < l; i++)
    if (p[i] == fromItem)
      std::string::replace(i, 1, _to);

  // retour
  return (*this);
}

//!remplacement d'une sous chaine de caracteres dans une chaine
/*!
  Cette methode recherche et remplace une sous-chaine de caracteres donnee par une autre dans une chaine de caracteres.
  \param fromItem sous-chaine de caracteres e remplacer dans la chaine
  \param to sous-chaine de caracteres de remplacement dans la chaine
  \return nouvelle chaine de caracteres avec la substitution
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String &String::replace(const String &fromItem, const String &to)
//-----------------------------------------------------------------------------
{
  size_type i;
  size_type pos = length();

  // boucle de recherche
  while ((i = rfind(fromItem, pos)) != npos)
  {
    std::string::replace(i, fromItem.length(), to);
    pos = i;
  }

  // retour
  return (*this);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres. La position de la sous-chaine est donnee par la valeur de l'long de depart et la longueur de la sous-chaine. La chaine d'origine est inchangee.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.subString(4,5); // b="haine"
  \endcode
  \param startpos position de depart dans la chaine de caracteres
  \param length longueur de la chaine e extraire
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::subString(int startpos, int length) const
//-----------------------------------------------------------------------------
{
  String ret;
  ret.assign(*this, startpos, length);
  return ret;
}

//!recherche d'un caractere dans une chaine
/*!
  Cette methode cherche un caractere dans une chaine de caracteres e partir d'une position donnee dans la chaine.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  printf("%d\n",a.search('e',1); // renvoie 19 ('e' e la 19 eme position)
  printf("%d\n",a.search('e',20); // renvoie -1 (non trouve)
  \endcode
  \param startpos position de depart de recherche dans la chaine
  \param c caractere e rechercher
  \return position du caractere dans la chaine, ou valeur -1 si ce caractere n'a pu etre trouve.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
int String::search(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int pos = find(c, startpos);
  if (pos == (int)npos)
    return -1;
  return pos;
}

//!recherche d'une sous chaine de caracteres dans une chaine
/*!
  Cette methode cherche une sous chaine de caracteres dans une chaine de caracteres e partir d'une position donnee dans la chaine.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  printf("%d\n",a.search("de",20); // renvoie 10 (position dans la chaine)
  printf("%d\n",a.search("remove",20); // renvoie -1 (non trouve)
  \endcode
  \param startpos position de depart de recherche dans la chaine
  \param substring sous chaine e rechercher e rechercher
  \return position de la sous-chaine dans la chaine, ou valeur -1 si cette sous-chaine n'a pu etre trouve.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
int String::search(const String &substring, int startpos) const
//-----------------------------------------------------------------------------
{
  int pos = find(substring, startpos);
  if (pos == (int)npos)
    return -1;
  return pos;
}

//!suppression d'une partie d'une chaine de caracteres
/*!
  Cette methode supprime une partie d'une chaine de caracteres e partir d'une position donnee et sur une longueur donnee. La chaine donnees est modifiee

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  a.remove(4,5); // a="ma c de caracteres"
  \endcode
  \param startpos position de depart de la partie e supprimer
  \param length longueur de la partie e supprimer
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::remove(int startpos, int length)
//-----------------------------------------------------------------------------
{
  std::string::replace(startpos, length, "");
}

//!suppression d'une partie d'une chaine de caracteres
/*!
  Cette methode supprime une partie d'une chaine de caracteres specifiee dans une autre chaine e partir d'une position de depart donnee. La chaine donnees est modifiee

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  a.remove("chaine",0); // a="ma  de caracteres"
  \endcode
  \param y sous-chaine e supprimer
  \param startpos position de depart pour la recherche dans la chaine
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::remove(const String &y, int startpos)
//-----------------------------------------------------------------------------
{
  std::string::replace(find(y, startpos), y.length(), "");
}

//!suppression d'un caractere dans une chaine
/*!
  Cette methode supprime un caractere specifie dans une autre chaine e partir d'une position de depart donnee. La chaine donnees est modifiee

Exemple:
  \code
  String a = "ma chaine de caracteres";
  a.remove('c',0); // a="ma haine de caracteres"
  \endcode
  \param y sous-chaine e supprimer
  \param startpos position de depart pour la recherche dans la chaine
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::remove(char c, int startpos)
//-----------------------------------------------------------------------------
{
  std::string::replace(find(c, startpos), 1, "");
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres. La position de la sous-chaine est donnee par la valeur de l'long de depart et la longueur de la sous-chaine.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.atItem(4,5); // b="haine"
  \endcode
  \param first position de depart dans la chaine de caracteres
  \param len longueur de la chaine e extraire
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::atItem(int first, int len) const
//-----------------------------------------------------------------------------
{
  return subString(first, len);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres. La position de la sous-chaine est donnee par la valeur de l'long de depart et la longueur de la sous-chaine.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a(4,5); // b="haine"
  \endcode
  \param first position de depart dans la chaine de caracteres
  \param len longueur de la chaine e extraire
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::operator()(int first, int len) const
//-----------------------------------------------------------------------------
{
  return subString(first, len);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres avant une position donnee.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.beforeItem(4); // b="ma c"
  \endcode
  \param pos position de fin dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::beforeItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(0, pos);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres avant une position donnee (caractere compris).

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.throughItem(4); // b="ma ch"
  \endcode
  \param pos position de fin dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::throughItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(0, pos + 1);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres apres une position donnee.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.afterItem(4); // b="haine de caracteres"
  \endcode
  \param pos position de depart dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::afterItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(pos + 1, length() - (pos + 1));
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres apres une position donnee (caractere compris).

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.fromItem(4); // b="chaine de caracteres"
  \endcode
  \param pos position de depart dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::fromItem(int pos) const
//-----------------------------------------------------------------------------
{
  return subString(pos, length() - pos);
}
//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres. La position de la sous-chaine est donnee par la premiere occurence d'une chaine donnee en parametres e partir d'une position donnee.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.atItem("haine",2); // b="haine"
  \endcode
  \param y sous-chaine de caracteres e rechercher
  \param startfirst position de depart dans la chaine de caracteres
  \param len longueur de la chaine e extraire
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::atItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  return subString(first, y.length());
}

//!extraction d'un caractere d'une chaine de caracteres
/*!
  Cette methode extrait un caractere dans une chaine de caracteres. La position du caractere est donnee par la premiere occurence dde ce caractere donne en parametre e partir d'une position donnee. 

C'est un peu idiot comme truc ca !!!

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.atItem('h',2); // b="h"
  \endcode
  \param y sous-chaine de caracteres e rechercher
  \param startfirst position de depart dans la chaine de caracteres
  \param len longueur de la chaine e extraire
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::atItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first == -1)
    return "";
  return subString(first, 1);
}
//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres avant une position donnee par la premiere occurence d'une chaine donnee en parametre de cette methode.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.beforeItem("chaine",2); // b="ma "
  \endcode
  \param y sous chaine e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::beforeItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(y, startpos);
  if (last == -1)
    return "";
  return subString(0, last);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres avant une position donnee par la premiere occurence d'un caractere donne en parametre de cette methode.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.beforeItem('c',2); // b="ma "
  \endcode
  \param c caractere e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::beforeItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(startpos, c);
  return subString(0, last);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres avant une position donnee par la premiere occurence d'une chaine donnee en parametre de cette methode. La sous chaine recherchee est incluse.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.throughItem("chaine",2); // b="ma chaine"
  \endcode
  \param y sous chaine e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::throughItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(y, startpos);
  if (last == -1)
    return "";
  if (last >= 0)
    last += y.length();
  return subString(0, last);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres avant une position donnee par la premiere occurence d'un caractere donne en parametre de cette methode. Le caractere recherche est inclus.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.throughItem('c',2); // b="ma c"
  \endcode
  \param c caractere e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::throughItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int last = search(startpos, c);
  if (last == -1)
    return "";
  if (last >= 0)
    last += 1;
  return subString(0, last);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres apres une position donnee par la premiere occurence d'une chaine donnee en parametre de cette methode.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.afterItem("chaine",2); // b=" de caracteres"
  \endcode
  \param y sous chaine e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::afterItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  if (first >= 0)
    first += y.length();
  return subString(first, length() - first);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres apres une position donnee par la derniere occurence d'une chaine donnee en parametre de cette methode.

  Exemple:
  \code
  String a = "ma chaine de caracteres de truc";
  String b= a.afterItem("de"); // b=" truc"
  \endcode
  \param y sous chaine e rechercher
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String
String::afterLast(const String &y) const
//-----------------------------------------------------------------------------
{
  int pos = rfind(y);
  if (pos == (int)npos)
    return "";
  pos += y.length();
  return atItem(pos, length());
}

//-----------------------------------------------------------------------------
String
String::beforeLast(const String &y) const
//-----------------------------------------------------------------------------
{
  int pos = rfind(y);
  if (pos == (int)npos)
    return "";
  pos += y.length();
  return atItem(0, pos - 1);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres apres une position donnee par la premiere occurence d'un caractere donne en parametre de cette methode.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.afterItem('c',2); // b="haine de caracteres"
  \endcode
  \param c caractere e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::afterItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first >= 0)
    first += 1;
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres apres une position donnee par la premiere occurence d'une chaine donnee en parametre de cette methode. La sous-chaine est incluse.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.fromItem("chaine",1); // b="chaine de caracteres"
  \endcode
  \param y sous chaine e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::fromItem(const String &y, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(y, startpos);
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

//!extraction d'une sous-chaine de caracteres d'une chaine de caracteres
/*!
  Cette methode extrait une sous-chaine de caracteres dans une chaine de caracteres apres une position donnee par la premiere occurence d'un caractere donne en parametre de cette methode.

  Exemple:
  \code
  String a = "ma chaine de caracteres";
  String b= a.fromItem('c',2); // b="chaine de caracteres"
  \endcode
  \param c caractere e rechercher
  \param startpos position de debut de recherche dans la chaine de caracteres
  \return sous chaine de caracteres extraite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
String String::fromItem(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  int first = search(startpos, c);
  if (first == -1)
    return "";
  return subString(first, length() - first);
}

//!remplit une chaine de caractere e partir d'un fichier
/*!
  Cette methode list une chaine de caracteres sur un fichier et retourne un String contenant cette chaine de caracteres. Les caracteres sont lus dans le fichier jusqu'au caractere de fin de ligne '\n'. Si la fin de fichier est atteinte, cette methode renvoie 0, sinon elle renvoie 1.Cette methode se trouve donc bien nommee.

  Exemple:
  \code
  FILE* pfile;
  String s;
  pfile=fopen("fich","r");
  while (s.scanFileLine(pfile)) cout << s << "\n";
  fclose(pfile);
  \endcode
  \param pfile fichier e lire
  \return booleen specifiant si il reste des caracteres e lire dans le fichier.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
bool String::scanFileLine(FILE *pfile)
//-----------------------------------------------------------------------------
{
  int c;
  *this = "";

  while ((c = getc(pfile)) != '\n')
  {
    if (c == EOF)
      return 0;
    *this += c;
  }

  return 1;
}

//!suppression des blancs inutiles
/*!
  Cette methode supprime les blancs en debut et fin d'une chaine de caracteres comme indique ci dessous:

  "  hello " -> "hello"
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::strip()
//-----------------------------------------------------------------------------
{
  size_type start;
  size_type stop;
  if ((start = find_first_not_of(" ")) == npos)
    return;
  if ((stop = find_last_not_of(" ")) == npos)
    return;

  *this = subString(start, stop - start + 1);
}

//!teste la presence d'une chaine dans une autre et renvoie la position
/*!
  Cette methode recherche une chaine de caracteres dans une autre en tenant compte ou non de la difference majuscules/minuscules. Cette methode e ete developpee pour bibView.
  \param substring chaine de caracteres e rechercher
  \param cas booleen permettant de specifier si on veut du sensitive search (true) ou non (false)
  \return position de la sous-chaine dans la chaine ou -1 en cas d'echec de la recherche
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
int String::containsWithCaseSub(const String &substring, bool cas) const
//-----------------------------------------------------------------------------
{
  int startpos = 0;
  const char *s = chars();
  const char *t = substring.chars();
  int sl = length();
  int tl = substring.length();

  if (sl > 0 && tl > 0)
  {
    if (startpos >= 0)
    {
      const char *lasts = &(s[sl - tl]);
      const char *lastt = &(t[tl]);
      const char *p = &(s[startpos]);

      while (p <= lasts)
      {
        const char *x = p++;
        const char *y = t;
        if (cas)
        {
          while (*x++ == *y++)
            if (y >= lastt)
              return --p - s;
        }
        else
        {
          while (strncasecmp(x++, y++, 1) == 0)
            if (y >= lastt)
              return --p - s;
        }
      }
    }
    else
    {
      const char *firsts = &(s[tl - 1]);
      const char *lastt = &(t[tl - 1]);
      const char *p = &(s[sl + startpos + 1]);

      while (--p >= firsts)
      {
        const char *x = p;
        const char *y = lastt;
        if (cas)
        {
          while (*x-- == *y--)
            if (y < t)
              return ++x - s;
        }
        else
        {
          while (strncasecmp(x--, y--, 1) == 0)
            if (y < t)
              return ++x - s;
        }
      }
    }
  }
  return -1;
}

//!teste la presence d'une chaine dans une autre
/*!
  Cette methode recherche une chaine de caracteres dans une autre en tenant compte ou non de la difference majuscules/minuscules. Cette methode e ete developpee pour bibView.
  \param substring chaine de caracteres e rechercher
  \param cas booleen permettant de specifier si on veut du sensitive search (true) ou non (false)
  \return booleen: true si la sous-chaine existe ou false sinon.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
bool String::containsWithCase(const String &substring, bool cas) const
//-----------------------------------------------------------------------------
{
  return (containsWithCaseSub(substring, cas) >= 0);
}

//!convertit une chaine de caracteres en un type char*
/*!
  Cette methode convertit une chaine de caracteres en un type char*.

  Exemple:
  \code
  String x;
  x="chain to convert";
  printf("%s",x.chars());
  \endcode
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
const char *String::chars() const
//-----------------------------------------------------------------------------
{
  return c_str();
}

//!teste la presence d'un caractere dans une chaine
/*!
  Cette methode teste si une chaine de caracteres contient un caractere donne.
  \param c caractere e rechercher
  \return true si le caractere existe, false dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
bool String::contains(char c, int startpos) const
//-----------------------------------------------------------------------------
{
  if (find(c, startpos) != npos)
    return true;
  return false;
}

//!recherche d'un caractere dans une chaine
/*!
  Cette methode cherche une sous-chaine de caracteres dans une chaine existante et retourne true, ou la valeur false si cette chaine n'existe pas.
  \param startpos position de depart pour la recherche
  \param substring sous chaine e rechercher
  \return true ou false selon le resultat de la recherche.
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
bool String::contains(const String &substring, int startpos) const
//-----------------------------------------------------------------------------
{
  if (find(substring, startpos) != npos)
    return true;
  return false;
}

//!ecrit une chaine de caracteres sur le flux de sortie
/*!
  Cette methode ecrit une chaine de caracteres sur le flux de donnees ostream. Si la chaine de caracteres est vide, "empty" est ecrit sur la sortie.

  \param s chaine de caracteres ecrite
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::dump(const String &s) const
//-----------------------------------------------------------------------------
{
  std::cout << *this << std::endl;
}

//!teste si une chaine de caracteres est vide
/*!
  Cette methode teste si une chaine de caracteres est vide.
  \return true si la chaine de caracteres est vide false dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
bool String::empty() const
//-----------------------------------------------------------------------------
{
  return ((*this) == "");
}

//!teste si une chaine de caracteres est non vide
/*!
  Cette methode teste si une chaine de caracteres est non vide.
  \return true si la chaine de caracteres est non vide false dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
bool String::ok() const
//-----------------------------------------------------------------------------
{
  return !(empty());
}

//!renvoie le premier caractere d'un String
/*!
  Cette methode retourne le premier caractere d'un std::string
  \return premier caractere du String
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
char String::firstchar() const
//-----------------------------------------------------------------------------
{
  return std::string::at(0);
}

//!renvoie le dernier caractere d'un String
/*!
  Cette methode retourne le dernier caractere d'un std::string
  \return dernier caractere du String
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
char String::lastchar() const
//-----------------------------------------------------------------------------
{
  return std::string::at(length() - 1);
}

//!conversion des caracteres en minuscules
/*!
  Cette methode convertit les caracteres d'une chaine en minuscules
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::lowerCase()
//-----------------------------------------------------------------------------
{
  int n = length();
  for (int i = 0; i < n; i++)
  {
    if ((std::string::at(i) >= 'A') && (std::string::at(i) <= 'Z'))
    {
      std::string::at(i) += 'a' - 'A';
    }
  }
}

//!conversion des caracteres en majuscules
/*!
  Cette methode convertit les caracteres d'une chaine en majuscules
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void String::upperCase()
//-----------------------------------------------------------------------------
{
  int n = length();
  for (int i = 0; i < n; i++)
  {
    if ((std::string::at(i) >= 'a') && (std::string::at(i) <= 'z'))
    {
      std::string::at(i) += 'A' - 'a';
    }
  }
}

//!convertit un String en un reel
/*!
  Cette methode extrait d'un String une valeur reelle e une colonne donnee.

  Exemple:
  \code
  String s="5.0 8.4 12.7e-6";
  double a,b,c;
  a=s.getRealAtPos(0); // a=5.0
  b=s.getRealAtPos(1); // b=8.4
  c=s.getRealAtPos(2); // c=12.7e-6
  \endcode
  \param pos specifie que l'on souhaite extraire la ieme valeur sur la ligne
  \param sep specifie le separateur de donnees sur la ligne que l'on veut utiliser (par defaut un espace si on ne precise rien)
  \return valeur reelle de retour convertie
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
double
String::getRealAtPos(int pos, String sep)
//-----------------------------------------------------------------------------
{
  double val;
  String tempo;
  tempo = *this;

  for (int i = 0; i < pos - 1; i++)
  {
    tempo.strip();
    tempo = tempo.afterItem(sep);
  }
  sscanf(tempo.chars(), "%lf", &val);
  return val;
}

//!extrait une valeur reelle d'un std::string
/*!
  Cette methode extrait d'un String une valeur reelle et reduit cette chaine de caracteres. Si elle ne contient plus de valeurs e extraire, cette methode renvoie false au moment de l'extraction de la derniere valeur de la ligne.

  Exemple:
  \code
  String s="5.0 8.4 12.7e-6";
  double a,b,c;
  bool r;
  r=s.popReal(a); // a=5.0, r=true
  r=s.popReal(b); // b=8.4, r=true
  r=s.popReal(c); // c=12.7e-6, r=false
  \endcode
  \param pos specifie que l'on souhaite extraire la ieme valeur sur la ligne
  \param sep specifie le separateur de donnees sur la ligne que l'on veut utiliser (par defaut un espace si on ne precise rien)
  \return valeur reelle de retour convertie
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
bool String::popReal(double &ret, String sep)
//-----------------------------------------------------------------------------
{
  strip();

  if (*this != "")
  {
    // la conversion du premier en un reel
    ret = getReal();

    // la recherche du suivant
    if (contains(sep))
    {
      *this = afterItem(sep);
      strip();
      return true;
    }
  }

  return false;
}

//!extrait une valeur entiere d'un std::string
/*!
  Cette methode extrait d'un String une valeur entiere et reduit cette chaine de caracteres. Si elle ne contient plus de valeurs e extraire, cette methode renvoie false au moment de l'extraction de la derniere valeur de la ligne.

  Exemple:
  \code
  String s="5 8 12";
  int a,b,c;
  bool r;
  r=s.popInt(a); // a=5, r=true
  r=s.popInt(b); // b=8, r=true
  r=s.popInt(c); // c=12, r=false
  \endcode
  \param pos specifie que l'on souhaite extraire la ieme valeur sur la ligne
  \param sep specifie le separateur de donnees sur la ligne que l'on veut utiliser (par defaut un espace si on ne precise rien)
  \return valeur reelle de retour convertie
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.6
*/
//-----------------------------------------------------------------------------
bool String::popInt(long &val, String sep)
//-----------------------------------------------------------------------------
{
  double rval = 0;
  bool ret;
  ret = popReal(rval, sep);
  val = (long)rval;
  return ret;
}

//!extrait un mot d'une chaine de caracteres
/*!
  Cette methode extrait un mot d'une chaine de caracteres. Cette methode retourne la valeur true s'il reste des mosts dans la chaine, false dans le cas contraire.
  \param ret mot extrait de la chaine de caracteres
  \param sep specifie le separateur de donnees sur la ligne que l'on veut utiliser (par defaut un espace si on ne precise rien)
  \return true si la chaine contient d'autres mots, false dans le cas contraire
  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
bool String::popString(String &ret, String sep)
//-----------------------------------------------------------------------------
{
  strip();

  if (*this != "")
  {

    // la recherche du suivant
    if (contains(sep))
    {
      ret = beforeItem(sep);
      *this = afterItem(sep);
      strip();
      return true;
    }
    ret = *this;
    return true;
  }
  return false;
}

//!conversion d'un String en un entier
//-----------------------------------------------------------------------------
long String::getInt() const
//-----------------------------------------------------------------------------
{
  return atoi(c_str());
}

//!conversion d'un String en un reel
//-----------------------------------------------------------------------------
double String::getReal() const
//-----------------------------------------------------------------------------
{
  return atof(c_str());
}

//!ajout d'une extension si elle n'est pas presente
//-----------------------------------------------------------------------------
void String::addExtension(String ext)
//-----------------------------------------------------------------------------
{
  bool present = false;

  // test if len is sufficient
  if (length() > ext.length())
  {
    std::cout << (*this)(length() - ext.length(), length()) << std::endl;
    if ((*this)(length() - ext.length(), length()) == ext)
    {
      present = true;
    }
  }

  // is not present, so add it !!
  if (!present)
    *this = *this + ext;
}

//-----------------------------------------------------------------------------
void String::fromFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // Read the length of the string as long
  long stringLen;
  long ret;
  ret = fread((void *)(&stringLen), sizeof(long), 1, pfile);

  // Read the string itself
  char stt[stringLen + 5];
  ret = fread(stt, sizeof(char), stringLen, pfile);

  // Add the CR to the string
  stt[stringLen] = '\0';

  (*this) = stt;
}

//-----------------------------------------------------------------------------
void String::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // Write the length of the string as long
  long stringLen = length();
  fwrite((void *)(&stringLen), sizeof(long), 1, pfile);

  // Write the string itself
  const char *p = data();
  fwrite((void *)(p), sizeof(char), stringLen, pfile);
}
