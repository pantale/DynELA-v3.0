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
 *   along with this program; if falset, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/

// begin date : 05/03/1997
// revision date : 16/03/2000

/*
 * Class List definition
 */

#ifndef __dnlKernel_List_h__
#define __dnlKernel_List_h__

#include <cstring>
#include <iostream>
#include <Errors.h>

/*!
  \file List.h
  \brief fichier .h de definition de la liste standard pour tout type d'objet
  \ingroup basicTools

  Ce fichier decrit les listes pour tous types d'objet.

  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/

#define DEFAULT_stack_size 10 //!< taille par defaut de la liste
#define DEFAULT_stack_inc 10  //!< increment par defaut pour la liste

template <class Type>
class ListIndex;

/*!
  \class List ListIndex.h
  \brief liste standard pour tout type d'objet
  \ingroup basicTools

  Cette classe permet de gerer tout type d'objet en memoire sous forme de liste de type vectorielle par l'intermediaire generalement des pointeurs sur les objets. Cette classe est un peu comme une classe de vecteurs que l'on utiliserait pour gerer les objets en memoire. Elle peut par exemple etre utilisee pour gerer une liste de falseeuds, une liste d'elements...

  Ce n'est pas une liste chainee, mais une liste basee sur une falsetion de vecteur avec une dimension dynamique. La taille de la liste est adaptee pour contenir le bon falsembre d'elements en memoire. L'utilisateur e acces e chacun des elements de la liste e travers l'index de cet element dans la liste ou e travers certaines methode particulieres de recherche d'element selon divers criteres. La partie de gestion des elements en memoire est totalement transparente pour l'utilisateur meme s'il peut "garder" un oeil sur les parametres ifluant cette gestion en memoire.

  Exemple:
  \code
  void test()
  {
  List <object*> listOfObjects;
  object* obj1=new object;
  listOfObjects << object;
  }
  \endcode
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
/** @dia:pos 128,4 */
template <class Type>
class List
{
    friend class ListIndex<Type>;

private:
    long sz;
    long s_size;
    long s_inc;
    long pcurrent;
    Type *ptr;

public:
    List(const long stack = DEFAULT_stack_size);
    virtual ~List();

    // member methods
public:
    void redim(const long taille);
    void close();
    Type &operator()(const long i);
    Type operator()(const long i) const;
    Type next();
    Type first();
    Type last();
    Type previous();
    Type currentUp();
    Type currentDown();
    Type current();
    long getSize() const;
    long stackSize() const;
    long &stackIncrement();
    List<Type> operator<<(const Type objet);
    void sort(bool (*funct)(const Type objet1, const Type objet2));
    Type dichotomySearch(long (*funct)(const Type objet1, const long i), const long i) const;
    void getInverse();
    void del(long ind);
    void del(long start, long stop);
    void delBefore(long ind);
    void delAfter(long ind);
    bool contains(const Type objet) const;

    // basic search method
    long getIndex(const Type objet) const;

    // comparison methods
    bool operator==(const List<Type> &objet) const;
    bool operator!=(const List<Type> &objet) const;
    long objectSize()
    {
        return (sizeof(*this) + s_size * sizeof(ptr));
    }

    // io methods
    void print(std::ostream &os) const;

    // virtual methods
    virtual void flush();
    virtual void add(const Type objet);
    virtual void insert(const Type objet, long ind);
};

/*!
  \class ListIndex List.h
  \brief classe de liste d'objets avec index.
  \ingroup basicTools
  \author &copy; Olivier PANTALE

  Cette classe permet de gerer tout type d'objet en memoire sous forme de liste de type vectorielle par l'intermediaire generalement des pointeurs sur les objets. Cette classe est un peu comme une classe de vecteurs que l'on utiliserait pour gerer les objets en memoire. Elle peut par exemple etre utilisee pour gerer une liste de falseeuds, une liste d'elements...

  Ce n'est pas une liste chainee, mais une liste basee sur une falsetion de vecteur avec une dimension dynamique. La taille de la liste est adaptee pour contenir le bon falsembre d'elements en memoire. L'utilisateur e acces e chacun des elements de la liste e travers l'index de cet element dans la liste ou e travers certaines methode particulieres de recherche d'element selon divers criteres. La partie de gestion des elements en memoire est totalement transparente pour l'utilisateur meme s'il peut "garder" un oeil sur les parametres ifluant cette gestion en memoire.

  Cette classe ListIndex est une extension de la classe List. Elle inclu plus de possibilites basees sur la presence d'un indice e l'interieur de chaque objet (\b falsemme \b _listIndex \b obligatoirement). A l'aide de cet index, on peut alors effecuer des operations de tri sur la liste et optimiser la recherche d'elements dans la liste.

  \warning L'utilisation de ce type de liste pour une classe oblige e inclure une variable \b _listIndex de type long dans la classe.

  Exemple:
  \code
  void test()
  {
  ListIndex <object*> listOfObjects;
  object* obj1=new object;
  listOfObjects << object;
  }
  \endcode
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/

template <class Type>
class ListIndex : public List<Type>
{
    bool sorted;
    bool comp;

public:
    ListIndex(const long stack = DEFAULT_stack_size);
    ~ListIndex();

    // member methods
    Type AppN(const long i) const;
    long IAppN(const long i) const;
    bool isSorted() const;
    bool isCompacted() const;
    void flush();
    void add(const Type objet);
    void sort();
    void forceSort();
    void sort(bool (*funct)(const Type objet1, const Type objet2));
    void compact();
    void del(const Type ind);
    void del(const Type start, const Type stop);
    void delBefore(const Type ind);
    void delAfter(const Type ind);
    void del(long ind)
    {
        List<Type>::del(ind);
    }
    void insert(const Type objet, long ind);
    void del(long start, long stop)
    {
        List<Type>::del(start, stop);
    }
    void delBefore(long ind)
    {
        List<Type>::delBefore(ind);
    }
    void delAfter(long ind)
    {
        List<Type>::delAfter(ind);
    }
};

//!constructeur par defaut de la classe List
/*!
  Ce constructeur alloue la memoire par defaut pour une instance de la classe List. Si la taille de la liste n'est pas precisee, la taille par defaut est prise en compte, celle ci est definie par la valeur de DEFAULT_stack_size.
  \param stack definit la taille initiale de la liste en falsembre d'objets
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
List<Type>::List(const long stack)
//-----------------------------------------------------------------------------
{
    // allocation des constantes
    s_size = stack;
    s_inc = DEFAULT_stack_inc;
    sz = 0;
    pcurrent = 0;

    // allocation memoire pour la liste
    ptr = new Type[s_size];

#ifdef VERIF_alloc
    if (ptr == NULL)
        fatalError("List<Type>::List",
                   "Memory allocation error\n"
                   "May be out of memory on this system\n");
#endif
}

//!extension de la taille d'une liste
/*!
  Cette methode est utilisee pour allonger ou reduire la taille d'une liste. Si la falseuvelle taille proposee est inferieure e la taille minimale necessaire pour stocker les elements actuels de la liste, une erreur est generee. Cette methode ne doit generalement pas etre appelee par l'utilisateur (sauf s'il a une totale maitreise du truc ;-0 ). Cette methode est fortement utilisee en interne par les autres methodes de la classe. Dans le cas oe l'utilisateur n'a pas la maitrise suffisante pour ce genre d'operation, il vaut mieux laisser la classe gerer elle meme ses allocations memoire.

  Une utilisation possible de cette methode se situe dans la pre-allocation memoire, quand on connait e l'avance le falsembre d'objets qui seront stockes dans la liste. On ajuste alors la taille de la liste e cette valeur ce qui evite les operations d'ajustement dynamique de taille coeteuses en temps CPU. Bien entendu, les mecanismes d'allocation dynamique existent et on peut depasser cette valeur.

  Exemple:
  \code
  void test()
  {
  List <object*> listOfObjects;
  listOfObjects.redim(1280); // 1280 objets e stocker
  for (i=0;i<1280);i++)
  {
  object* obj1=new object;
  listOfObjects << obj1; // stockage sans aucune reallocation dynamique
  }
  object* obj2=new object;
  listOfObjects << obj2; // et Oups, un 1281 eme
  }
  \endcode
  \param size_ definit la falseuvelle taille de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::redim(const long size_)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (size_ < sz)
        fatalError("template <class Type> void List<Type>::redim(const long)\n",
                   "new getSize < actual getSize means to truncate list\n"
                   "falset allowed with redim method\n"
                   "Must use a 'del' method");
#endif

    // affectation de la size_
    s_size = size_;

    // allocation de la zone memoire
    Type *ptr2 = new Type[s_size];

#ifdef VERIF_alloc
    if (ptr2 == NULL)
        fatalError("List<Type>::redim",
                   "memory allocation error\nMay be out of memory on this system\n");
#endif

    // recopie de la zone memoire
    memcpy(ptr2, ptr, sz * sizeof(Type));

    // destruction de l'ancien pointeur
    delete[] ptr;

    // reaffectation du pointeur
    ptr = ptr2;
}

//!ajuste la taille courante d'une liste e la taille reelle
/*!
  Cette methode est utilisee pour ajuster la taille de la liste en fonction du falsembre d'objets reels contenus dans la liste. Cette methode permet alors de recuperer de l'espace memoire, principalement pour les petites listes.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::close()
//-----------------------------------------------------------------------------
{
    // on fait un redim a la size_ reelle de la pile
    redim(sz);
}

//!accesseur aux elements de la liste
/*!
  Cette methode est utilisee pour acceder aux elements de la liste. Cet acces est e la fois en lecture et en ecriture. Cette methode retourne l'element [i] de la liste. La base de reference est 0 (permier element d'indice 0) comme habituellement en C et C++.
  \param ind numero de l'element dans la liste
  \return un element de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type &List<Type>::operator()(const long ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((ind < 0) || (ind >= sz))
    {
        std::cerr << "Fatal Error in template <class Type> Type& List<Type>::operator ()\n";
        std::cerr << "long " << ind << " out of allowd range {0-" << sz - 1 << "}\n";
        exit(-1);
    }
#endif
    return ptr[pcurrent = ind];
}

//!accesseur aux elements de la liste
/*!
  Cette methode est utilisee pour acceder aux elements de la liste. Cet acces est en lecture seule. Cette methode retourne l'element [i] de la liste. La base de reference est 0 (permier element d'indice 0) comme habituellement en C et C++.
  \param ind numero de l'element dans la liste
  \return un element de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::operator()(const long ind)
    const
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if ((ind < 0) || (ind >= sz))
    {
        std::cerr << "Fatal Error in template <class Type> Type& List<Type>::operator ()\n";
        std::cerr << "long " << ind << " out of allowd range {0-" << sz - 1 << "}\n";
        exit(-1);
    }
#endif
    return ptr[ind];
}

//!renvoie l'element suivant dans la liste
/*!
  Cette methode utilise un mecanisme de reperage interne dans la liste pour renvoyer l'element suivant le precedent appel dans la liste. Pour utiliser cette methode, il convient de bien cerner les bornes de la liste, et d'avoir bien reference le depart par les methodes first(), last() ou les accesseurs ().
  \return element suivant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::next()
//-----------------------------------------------------------------------------
{
    if (pcurrent >= sz - 1)
    {
        return NULL;
    }
    return ptr[++pcurrent];
}

//!renvoie l'element suivant dans la liste
/*!
  Cette methode utilise un mecanisme de reperage interne dans la liste pour renvoyer l'element suivant le precedent appel dans la liste. Pour utiliser cette methode, il convient de bien cerner les bornes de la liste, et d'avoir bien reference le depart par les methodes first(), last() ou les accesseurs ().
  \return element suivant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::currentUp()
//-----------------------------------------------------------------------------
{
    if (pcurrent >= sz)
    {
        return NULL;
    }

    return ptr[pcurrent++];
}

//!renvoie l'element suivant dans la liste
/*!
  Cette methode utilise un mecanisme de reperage interne dans la liste pour renvoyer l'element suivant le precedent appel dans la liste. Pour utiliser cette methode, il convient de bien cerner les bornes de la liste, et d'avoir bien reference le depart par les methodes first(), last() ou les accesseurs ().
  \return element suivant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::currentDown()
//-----------------------------------------------------------------------------
{
    if (pcurrent < 0)
    {
        return NULL;
    }

    return ptr[pcurrent--];
}

//!renvoie le premier element de la liste
/*!
  Cette methode retourne le premier element de la liste.
  \return premier element dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::first()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent = 0];
}

//!renvoie le dernier element de la liste
/*!
  Cette methode retourne le dernier element de la liste.
  \return dernier element dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::last()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent = sz - 1];
}

//!renvoie l'element precedent dans la liste
/*!
  Cette methode utilise un mecanisme de reperage interne dans la liste pour renvoyer l'element precedent du "precedent appel" dans la liste. Pour utiliser cette methode, il convient de bien cerner les bornes de la liste, et d'avoir bien reference le depart par les methodes first(), last() ou les accesseurs ().
  \return element precedent dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::previous()
//-----------------------------------------------------------------------------
{
    if (pcurrent == 0)
    {
        return NULL;
    }
    return ptr[--pcurrent];
}

//!element courant dans la liste
/*!
  Cette methode utilise un mecanisme de reperage interne dans la liste pour renvoyer l'element courant du "precedent appel" dans la liste. Pour utiliser cette methode, il convient de bien cerner les bornes de la liste, et d'avoir bien reference le depart par les methodes first(), last() ou les accesseurs ().
  \return element courant dans la liste ou NULL si celui-ci n'existe pas.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::current()
//-----------------------------------------------------------------------------
{
    if (sz == 0)
    {
        return NULL;
    }
    return ptr[pcurrent];
}

//!taille de la liste
/*!
  Cette methode retourne la taille de la liste, c'est e dire le falsembre exact d'elements dans la liste. Les indices varient dans l'intervalle [0:sz-1]
  \return taille de la liste
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
long List<Type>::getSize() const
//-----------------------------------------------------------------------------
{
    return sz;
}

//!taille de la pile
/*!
  Cette methode retourne la taille de la pile, c'est e dire le falsembre maxi d'elements dans la liste avant la prochaine reallocation dynamique.
  \return taille de la pile
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
long List<Type>::stackSize() const
//-----------------------------------------------------------------------------
{
    return s_size;
}

//!valeur de l'increment de pile
/*!
  Cette methode est utilisee pour contreler la valeur de l'increment de pile. A la prochaine allocation memoire pour la pile, cette valeur sera utilisee pour determiner de quelle taille doit etre augmentee la pile. Une grande valeur permet d'obtenir un code plus rapide (car elle diminue la frequence des allocations memoire), mais elle est egalement plus coeteuse en terme de memoire allouee.
  \return valeur de l'increment de pile
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
long &List<Type>::stackIncrement()
//-----------------------------------------------------------------------------
{
    return s_inc;
}

//!vide la contenu de la pile
/*!
  Cette methode vide le contenu de la pile et ramene sa taille reelle e zero et sa taille de pile e DEFAULT_stack_size. La pile est comme neuve !!! (c'est une pile rechargeable ;-] )
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::flush()
//-----------------------------------------------------------------------------
{
    s_size = DEFAULT_stack_size;
    sz = 0;
    delete[] ptr;
    pcurrent = 0;

    ptr = new Type[s_size];
#ifdef VERIF_alloc
    if (ptr == NULL)
        fatalError("template <class Type> void List<Type>::flush()",
                   "fonction flush de template <class Type> List<Type>\n");
#endif
}

//!ajoute un objet e la liste
/*!
  Cette methode ajoute un objet e la liste. L'objet est ajoute e la fin de la liste, et la taille de liste est automatiquement incrementee si besoin.
  \param objet e rajouter e la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
List<Type> List<Type>::operator<<(const Type object)
//-----------------------------------------------------------------------------
{
    add(object);
    return *this;
}

//!insere un element dans la liste
/*!
  Cette methode ajoute un objet e la liste. L'objet est insere au milieu de la liste, et la taille de liste est automatiquement incrementee si besoin.
  \param ind definit la position de l'insertion dans la liste
  \param objet e rajouter e la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::insert(const Type objet, long ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (ind > sz)
        fatalError("List<Type>::insert(long ind)",
                   "position indice (%d) out of bounds (%d)\n", ind, sz);
#endif

    // add the last object at the end
    add(ptr[sz - 1]);

    // move from the end to the insertion point
    if (sz - 2 >= ind)
    {
        memmove(ptr + ind + 1, ptr + ind, (sz - ind - 2) * sizeof(Type));
    }

    // insert the object
    ptr[ind] = objet;
}

//!insere un element dans la liste
/*!
  Cette methode ajoute un objet e la liste. L'objet est insere au milieu de la liste, et la taille de liste est automatiquement incrementee si besoin.
  \param ind definit la position de l'insertion dans la liste
  \param objet e rajouter e la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::insert(const Type objet, long ind)
//-----------------------------------------------------------------------------
{
    List<Type>::insert(objet, ind);
    sorted = false;
    comp = false;
}

//!ajoute un objet e la liste
/*!
  Cette methode ajoute un objet e la liste. L'objet est ajoute e la fin de la liste, et la taille de liste est automatiquement incrementee si besoin.
  \param objet e rajouter e la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // test de reallocation memoire
    if (sz >= s_size)
    {
        redim(s_size + s_inc);
    }

    // stockage du courrant
    pcurrent = sz;

    // ajout de l'objet
    ptr[sz++] = object;
}

//!getInverse la liste
/*!
  Cette methode getInverse l'ordre des elements dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::getInverse()
//-----------------------------------------------------------------------------
{
    Type v;
    for (long i = 0; i < sz / 2; i++)
    {
        v = ptr[i];
        ptr[i] = ptr[sz - i - 1];
        ptr[sz - i - 1] = v;
    }
}

//!supprime un ensemble d'elements dans la liste
/*!
  Cette methode supprime un ensemble d'elements dans la liste. Cette methode est utilisee pour supprimer tout un segment de la liste, en definissant les indices de depart et d'arrivee du segment dans la liste. Si les parametres start et stop sont egaux, un seul element est supprime.
  \param start premier element e supprimer
  \param stop dernier element e supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::del(long start, long stop)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (start > stop)
        fatalError("List<Type>::del(long start,long stop)",
                   "start indice %d > stop indice %d\n", start, stop);
    if (stop >= sz)
        fatalError("List<Type>::del(long start,long stop)",
                   "stop indice (%d) out of bounds (%d)\n", stop, sz);
#endif

    // recouvrement
    if (sz - stop - 1 > 0)
    {
        memmove(ptr + start, ptr + (stop + 1), (sz - stop - 1) * sizeof(Type));
    }

    // recalcul de la size_
    sz -= (stop - start + 1);
}

//!supprime un element de la liste
/*!
  Cette methode supprime un element dans la liste. Cette methode est utilisee pour supprimer un seul element de la liste, en definissant l'indice de l'element e supprimer
  \param ind element e supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::del(long ind)
//-----------------------------------------------------------------------------
{
    List<Type>::del(ind, ind);
}

//!supprime les elements avant un indice donne
/*!
  Cette methode supprime tous les elements de la liste compris entre le debut de la liste et la valeur donnee en argument de cette methode. Cette methode est equivalente e del(0,ind-1).
  \param ind definit le premier element de la liste e garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::delBefore(long ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (ind - 1 >= sz)
        fatalError("List<Type>::delBefore(long ind)",
                   "ind indice (%d) out of bounds (%d)\n", ind, sz);
#endif

    List<Type>::del(0, ind - 1);
}

//!supprime les elements apres un indice donne
/*!
  Cette methode supprime tous les elements de la liste compris entre la valeur donnee en argument de cette methode et la fin de la liste. Cette methode est equivalente e del(ind+1,last()).
  \param ind definit le dernier element de la liste e garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::delAfter(long ind)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_bounds
    if (ind + 1 >= sz)
        fatalError("List<Type>::delAfter(long ind)",
                   "ind indice (%d) out of bounds (%d)\n", ind, sz);
#endif

    List<Type>::del(ind + 1, sz - 1);
}

//!comparaison de deux listes
/*!
  Cette methode est utilisee pour comparer deux listes entre elles. Elle teste l'egalite.
  \param liste de comparaison
  \return true si les deux listes sont egales, false dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List<Type>::operator==(const List<Type> &liste) const
//-----------------------------------------------------------------------------
{
    // la comparaison porte sur la size_
    if (sz != liste.sz)
    {
        return (false);
    }

    // et sur le contenu
    for (long i = 0; i < sz; i++)
        if (ptr[i] != liste.ptr[i])
        {
            return (false);
        }

    return (true);
}

//!comparaison de deux listes
/*!
  Cette methode est utilisee pour comparer deux listes entre elles. Elle teste la falsen egalite.
  \param liste de comparaison
  \return false si les deux listes sont egales, true dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List<Type>::operator!=(const List<Type> &liste) const
//-----------------------------------------------------------------------------
{
    return !(*this == liste);
}

//!affichage des elements de la liste
/*!
  Cette methode affiche les elements de la liste. Elle est utilisee e des fins de debogage.
  \warning Les objets geres par la liste doivent avoir une methode \b << permettant l'affichage de leur contenu.
  \param os flux \c ostream de sortie
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::print(std::ostream &os) const
//-----------------------------------------------------------------------------
{
    os << "list " << sz << "/" << s_size << "={";
    for (long i = 0; i < sz; i++)
    {
        if (i != 0)
        {
            std::cout << ",";
        }
        os << *ptr[i];
    }
    os << "}";
}

//!methode de recherche dans la liste
/*!
  Cette methode effectue une recherche simple d'un element dans la liste et renvoie un Index indiquant la place de l'objet dans la liste. Si l'objet n'est pas trouve, elle retourne la valeur -1.
  \param objet objet e rechercher dans la liste
  \return Index de l'objet dans la liste
  \author &copy; Olivier PANTALE
  \version 1.0.0
  \date 2002
*/
//-----------------------------------------------------------------------------
template <class Type>
long List<Type>::getIndex(const Type objet) const
//-----------------------------------------------------------------------------
{
    // recherche bourrin de base
    for (long i = 0; i < sz; i++)
    {
        if (ptr[i] == objet)
        {
            //	  pcurrent=i;
            return i;
        }
    }

    // bourrin pas trouve alors on retourne -1
    return -1;
}

//!methode de recherche dans la liste
/*!
  Cette methode effectue une recherche simple d'un element dans la liste et renvoie un booleen selon la presence ou falsen de cet objet dans la liste.
  \param objet objet e rechercher dans la liste
  \return true si l'objet est dans la liste, false dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
bool List<Type>::contains(const Type objet) const
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < sz; i++)
    {
        if (ptr[i] == objet)
        {
            return true;
        }
    }
    return false;
}

//!constructeur par defaut de la classe ListIndex
/*!
  Ce constructeur alloue la memoire par defaut pour une instance de la classe List. Si la taille de la liste n'est pas precisee, la taille par defaut est prise en compte, celle ci est definie par la valeur de DEFAULT_stack_size.
  \param stack definit la taille initiale de la liste en falsembre d'objets
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
ListIndex<Type>::ListIndex(const long stack) : List<Type>(stack)
//-----------------------------------------------------------------------------
{
    // allocation des constantes
    sorted = true;
    comp = true;
}

//!destructeur de la classe List
//-----------------------------------------------------------------------------
template <class Type>
List<Type>::~List()
//-----------------------------------------------------------------------------
{
    // vide, ici il semble que l'on ait rien a faire
}

//!destructeur de la classe ListIndex
//-----------------------------------------------------------------------------
template <class Type>
ListIndex<Type>::~ListIndex()
//-----------------------------------------------------------------------------
{
    // vide, ici il semble que l'on ait rien a faire
}

//!recherche d'une element dans la liste
/*!
  Cette methode est utilisee pour rechercher un element dans la liste en utilisant un algorithme dichotomique. Cette methode retourne l'element correspondant dans la liste ou la valeur NULL si l'element n'est pas dans la liste.
  Exemple:
  \code
  class truc
  {
    public:
    long z; // une valeur
  };
  ListIndex <truc*> listeTrucs; // la liste
  long compare(truc* p1, long in) // la fonction de comparaison
  {
    return (p1->z - in); // comparaison
  }
  ...
  {
  ...
  listeTrucs.sort(compare,10); // cherche la valeur 10
  }
  \endcode
  \warning Cette methode est uniquement valable si la liste est triee sur le parametre de recherche.
  \param funct fonction definissant la methode de comparaison e utiliser
  \param ind valeur particuliere de l'element e rechercher dans la liste
  \return l'element correspondant dans la liste ou la valeur NULL si l'element n'est pas dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type List<Type>::dichotomySearch(long (*funct)(const Type objet1, const long in), const long ind) const
//-----------------------------------------------------------------------------
{
    long i;

    // si la size_ est nulle, on retourne NULL
    if (this->sz == 0)
    {
        return NULL;
    }

    // tri dichotomique
    long g, d, dx;
    g = 0;
    d = this->sz - 1;
    do
    {
        i = (g + d) / 2;
        dx = funct(this->ptr[i], ind);
        if (dx == 0)
        {
            return this->ptr[i];
        }
        if (dx > 0)
        {
            d = i - 1; // a droite de i
        }
        else
        {
            g = i + 1; // a gauche de i
        }
    } while (g <= d);

    return NULL;
}

//!recherche d'une element dans la liste
/*!
  Cette methode est utilisee pour rechercher un element dans la liste. L'algorithme de recherche est base sur le parametre \b _listIndex declare dans les objets de la liste. Cette methode retourne l'element correspondant dans la liste ou la valeur NULL si l'element n'est pas dans la liste.
  \param ind index de l'element e rechercher dans la liste
  \return l'element correspondant dans la liste ou la valeur NULL si l'element n'est pas dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
Type ListIndex<Type>::AppN(const long ind) const
//-----------------------------------------------------------------------------
{
    long i;

    // si la size_ est nulle, on retourne NULL
    if (this->sz == 0)
    {
        return NULL;
    }

    // si elle est triee et compactee
    if (comp == true)
        if ((ind >= 0) && (ind < this->sz))
        {
            return this->ptr[ind];
        }

    // recherche selon le cas
    if (sorted == true)
    {
        // tri dichotomique
        long g, d;
        g = 0;
        d = this->sz - 1;
        do
        {
            i = (g + d) / 2;
            if (this->ptr[i]->_listIndex == ind)
            {
                return this->ptr[i];
            }
            if (ind < this->ptr[i]->_listIndex)
            {
                d = i - 1; // a droite de i
            }
            else
            {
                g = i + 1; // a gauche de i
            }
        } while (g <= d);
    }
    else
    {
        // tri bete mais terriblement efficace falsen !!
        for (i = 0; i < this->sz; i++)
            if (this->ptr[i]->_listIndex == ind)
            {
                return this->ptr[i];
            }
    }
    return NULL;
}

//!recherche d'une element dans la liste
/*!
  Cette methode est utilisee pour rechercher un element dans la liste. L'algorithme de recherche est base sur le parametre \b _listIndex declare dans les objets de la liste. Cette methode retourne l'indice de l'element correspondant dans la liste ou la valeur 0 si l'element n'est pas dans la liste.
  \warning La valeur de retour 0 peut etre confondue avec la premiere valeur de la liste ??? BUG ???
  \param ind index de l'element e rechercher dans la liste
  \return indice de l'element correspondant dans la liste ou la valeur 0 si l'element n'est pas dans la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
long ListIndex<Type>::IAppN(const long ind) const
//-----------------------------------------------------------------------------
{
    long i;

    // si la size_ est nulle, on retourne NULL
    if (this->sz == 0)
    {
        return 0;
    }

    // si elle est triee et compactee
    if (comp == true)
        if ((ind >= 0) && (ind < this->sz))
        {
            return (ind);
        }

    // recherche selon le cas
    if (sorted == true)
    {
        // tri dichotomique
        long g, d;
        g = 0;
        d = this->sz - 1;
        do
        {
            i = (g + d) / 2;
            if (this->ptr[i]->_listIndex == ind)
            {
                return (i);
            }
            if (ind < this->ptr[i]->_listIndex)
            {
                d = i - 1; // a droite de i
            }
            else
            {
                g = i + 1; // a gauche de i
            }
        } while (g <= d);
    }
    else
    {
        // tri bete
        for (i = 0; i < this->sz; i++)
            if (this->ptr[i]->_listIndex == ind)
            {
                return (i);
            }
    }
    return 0;
}

//!teste si la liste est triee
/*!
  Cette methode teste si la liste est actuellement triee.
  \return true si la liste est triee false dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
bool ListIndex<Type>::isSorted() const
//-----------------------------------------------------------------------------
{
    return sorted;
}

//!teste si la liste est compactee
/*!
  Cette methode teste si la liste est actuellement compactee. La falsetion de compaction est liee au fait que les indices de la liste sont contigus les uns aux autres en ordre croissant sans aucun "trou".
  \return true si la liste est compactee false dans le cas contraire
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
bool ListIndex<Type>::isCompacted() const
//-----------------------------------------------------------------------------
{
    return comp;
}

//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::flush()
//-----------------------------------------------------------------------------
{
    this->s_size = DEFAULT_stack_size;
    this->sz = 0;
    sorted = true;
    comp = true;
    delete[] this->ptr;
    this->pcurrent = 0;

    this->ptr = new Type[this->s_size];
#ifdef VERIF_alloc
    if (this->ptr == NULL)
        fatalError("template <class Type> void ListIndex<Type>::flush()",
                   "fonction flush de template <class Type> ListIndex<Type>\n");
#endif
}

//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::add(const Type object)
//-----------------------------------------------------------------------------
{
    // test de reallocation memoire
    if (this->sz >= this->s_size)
    {
        this->redim(this->s_size + this->s_inc);
    }

    // test de tri
    if (sorted == true)
    {
        if (this->sz != 0)
        {
            if (this->ptr[this->sz - 1]->_listIndex > object->_listIndex)
            {
                sorted = false;
                comp = false;
            }

            // test de comp
            if (object->_listIndex - this->ptr[this->sz - 1]->_listIndex != 1)
            {
                comp = false;
            }
        }
        else
        {
            if (object->_listIndex != 0)
            {
                comp = false;
            }
        }
    }
    // stockage du courrant
    this->pcurrent = this->sz;

    // ajout de l'objet
    this->ptr[this->sz++] = object;
}

//!trie la liste
/*!
  Cette methode trie les elements de la liste en fonction de l'index \b _listIndex contenu dans chaque element de la liste. Cette methode force le tri de la pile, meme ci celle-ci semble deja triee.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::forceSort()
//-----------------------------------------------------------------------------
{
    sorted = false;
    sort();
}

//!trie la liste
/*!
  Cette methode trie les elements de la liste en fonction de l'index \b _listIndex contenu dans chaque element de la liste.
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::sort()
//-----------------------------------------------------------------------------
{
    if (sorted == true)
    {
        return;
    }

    Type v;
    long i, j;
    long h = 1;
    while (h <= this->sz)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (long)(h / 3);
        for (i = h + 1; i <= this->sz; i++)
        {
            v = this->ptr[i - 1];
            j = i;
            while (this->ptr[j - h - 1]->_listIndex > v->_listIndex)
            {
                this->ptr[j - 1] = this->ptr[j - h - 1];
                j = j - h;
                if (j <= h)
                {
                    break;
                }
            }
            this->ptr[j - 1] = v;
        }
    }

    // maintenant elle est triee
    sorted = true;
}

//!tri de la liste e partir d'une fonction de comparaison
/*!
  Cette methode trie les elements de la pile en utilisant une fonction de comparaison definie par l'utilisateur. Cette methode est tres performante pour trier une liste et tres souple d'utilisation. L'utilisation peut sembler complexe, mais elle est definie dans l'exemple ci-dessous.

  Exemple:
  \code
  class truc
  {
    public:
    double z; // une valeur
  };
  ListIndex <truc*> listeTrucs; // la liste
  bool compare(truc* p1,truc* p2) // la fonction de comparaison
  {
    return (p1->z < p2->z); // comparaison
  }
  ...
  {
  ...
  listeTrucs.sort(compare); // tri selon la fonction de comparaison
  }
  \endcode
  \param funct fonction definissant la methode de comparaison e utiliser
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void List<Type>::sort(bool (*funct)(const Type objet1, const Type objet2))
//-----------------------------------------------------------------------------
{
    Type v;
    long i, j;
    long h = 1;
    while (h <= this->sz)
    {
        h = 3 * h + 1;
    }
    while (h != 1)
    {
        h = (long)(h / 3);
        for (i = h + 1; i <= this->sz; i++)
        {
            v = this->ptr[i - 1];
            j = i;
            while (funct(this->ptr[j - h - 1], v))
            {
                this->ptr[j - 1] = this->ptr[j - h - 1];
                j -= h;
                if (j <= h)
                {
                    break;
                }
            }
            this->ptr[j - 1] = v;
        }
    }
}

//!compactage de la liste
/*!
  Cette methode compacte la liste. La falsetion de compaction est liee au fait que les indices de la liste sont contigus les uns aux autres en ordre croissant sans aucun "trou".
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::compact()
//-----------------------------------------------------------------------------
{
    for (long i = 0; i < this->sz; i++)
    {
        this->ptr[i]->_listIndex = i;
    }

    // maintenant elle est triee et compactee
    comp = true;
    sorted = true;
}

//!supprime un ensemble d'elements dans la liste
/*!
  Cette methode supprime un ensemble d'elements dans la liste. Cette methode est utilisee pour supprimer tout un segment de la liste, en definissant les indices de depart et d'arrivee du segment dans la liste. Si les parametres start et stop sont egaux, un seul element est supprime.
  \param start premier element e supprimer
  \param stop dernier element e supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::del(const Type start, const Type stop)
//-----------------------------------------------------------------------------
{
    List<Type>::del(IAppN(start->_listIndex), IAppN(stop->_listIndex));
}

//!supprime un element de la liste
/*!
  Cette methode supprime un element dans la liste. Cette methode est utilisee pour supprimer un seul element de la liste, en definissant l'indice de l'element e supprimer
  \param ind element e supprimer
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::del(const Type ind)
//-----------------------------------------------------------------------------
{
    long i = IAppN(ind->_listIndex);
    List<Type>::del(i, i);
}

//!supprime les elements avant un indice donne
/*!
  Cette methode supprime tous les elements de la liste compris entre le debut de la liste et la valeur donnee en argument de cette methode. Cette methode est equivalente e del(0,ind-1).
  \param ind definit le premier element de la liste e garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::delBefore(const Type ind)
//-----------------------------------------------------------------------------
{
    List<Type>::del(0, IAppN(ind->_listIndex) - 1);
}

//!supprime les elements apres un indice donne
/*!
  Cette methode supprime tous les elements de la liste compris entre la valeur donnee en argument de cette methode et la fin de la liste. Cette methode est equivalente e del(ind+1,last()).
  \param ind definit le dernier element de la liste e garder
  \author &copy; Olivier PANTALE
  \version 0.9.5
  \date 1997-2004
*/
//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::delAfter(const Type ind)
//-----------------------------------------------------------------------------
{
    List<Type>::del(IAppN(ind->_listIndex) + 1, this->sz - 1);
}

//-----------------------------------------------------------------------------
template <class Type>
void ListIndex<Type>::sort(bool (*funct)(const Type objet1, const Type objet2))
//-----------------------------------------------------------------------------
{
    List<Type>::sort(*funct);
    sorted = false;
}

#endif
