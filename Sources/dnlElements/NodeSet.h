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

#ifndef __dnlElements_NodeSet_h__
#define __dnlElements_NodeSet_h__

#include <String.h>
#include <List.h>
#include <Node.h>

class NodeSet
{
public:
  String name;             //!< Public name of the NodeSet
  ListIndex<Node *> nodes; //!< List of the nodes

public:
  NodeSet(char *newName = NULL);   //! Constructor of the NodeSet class
  NodeSet(const NodeSet &nodeSet); //! Copy constructor of the NodeSet class
  ~NodeSet();                      //! Destructor of the NodeSet class

  void add(Node *node);
  void setName(char *_name);
  void add(NodeSet *);
  long getSize();
  Node *getNode(long item);
  long getNumber(long item);
  void intersect(NodeSet *nodeSet);
  void substract(NodeSet *nodeSet);
  void clear();

  // fonctions membres
  /*   
  void surface(ElementSet *); */
};

#endif
