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

#include <ElementSet.h>
#include <Errors.h>
//#include <DynELA.h>

//constructeur de la classe ElementSet
/*!
  Cette methode construit un ElementSet vide.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
ElementSet::ElementSet(char *_name)
//-----------------------------------------------------------------------------
{
  if (_name != NULL)
    setName(_name);
}


//constructeur par recopie de la classe ElementSet
/*!
  Cette methode construit un ElementSet vide par recopie d'un autre. 
  Elle n'est pas finalisee.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
ElementSet::ElementSet(const ElementSet &X)
//-----------------------------------------------------------------------------
{
  std::cerr << "can't copy directely ElementSet class" << /*(int) */ &X << std::endl;
  exit(-1);
}

//destructeur de la classe ElementSet
/*!
  Cette methode detruit un ElementSet.
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
ElementSet::~ElementSet()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ElementSet::setName(char *_name)
//-----------------------------------------------------------------------------
{
    name = _name;
}

//ajoute un element e un ElementSet
/*!
  Cette methode ajoute un element existant e un ElementSet. L'element est directement specifie par son pointeur, aucune verification n'est effectuee par cette methode.
  \param element pointeur sur un element
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
void ElementSet::add(Element *element)
//-----------------------------------------------------------------------------
{
  elements << element;
}

//nombre d'elements dans la liste
/*!
  Cette methode renvoie le nombre d'elements dans un ElementSet
  \return nombre d'elements de la liste
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
long ElementSet::getSize()
//-----------------------------------------------------------------------------
{
  return elements.getSize();
}

//ajoute un ElementSet e un ElementSet
/*!
  Cette methode ajoute un ElementSet existant e un ElementSet.
  \param elementSet ElementSet a fusionner
  \author Olivier PANTALE
  \version 1.0.0
*/
//-----------------------------------------------------------------------------
void ElementSet::add(ElementSet *elementSet)
//-----------------------------------------------------------------------------
{
  for (long i = 0; i < elementSet->elements.getSize(); i++)
  {
    elements << elementSet->elements(i);
  }
}

//recupere un pointeur sur le ieme element d'un ElementSet
/*!
  Cette methode renvoie un pointeur sur le ieme element d'une liste d'elements contenue dans un ElementSet.
  \param item place de l'element dans la liste.
  \return pointeur sur le element
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
Element *ElementSet::getElement(long item)
//-----------------------------------------------------------------------------
{
  return (elements(item));
}

//recupere le numero du ieme noeud d'un node set
/*!
  Cette methode renvoie le numero du ieme noeud d'une liste de noeuds contenue dans un NodeSet.
  \param i place du noeud dans la liste.
  \return pointeur sur le noeud
  \author Olivier PANTALE
  \version 0.9.5
*/
//-----------------------------------------------------------------------------
long ElementSet::getNumber(long item)
//-----------------------------------------------------------------------------
{
  return (elements(item)->number);
}

//-----------------------------------------------------------------------------
void ElementSet::intersect(ElementSet *elementSet)
//-----------------------------------------------------------------------------
{
  bool ok;
  for (long i = elements.getSize() - 1; i >= 0; i--)
  {
    ok = false;
    for (long j = 0; j < elementSet->elements.getSize(); j++)
    {
      if (elements(i) == elementSet->elements(j))
      {
        ok = true;
        break;
      }
    }
    if (!ok)
      elements.del(i);
  }
}

//-----------------------------------------------------------------------------
void ElementSet::substract(ElementSet *elementSet)
//-----------------------------------------------------------------------------
{
  for (long i = elements.getSize() - 1; i >= 0; i--)
  {
    for (long j = 0; j < elementSet->elements.getSize(); j++)
    {
      if (elements(i) == elementSet->elements(j))
      {
        elements.del(i);
        break;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void ElementSet::clear()
//-----------------------------------------------------------------------------
{
  elements.flush();
}
