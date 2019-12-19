/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Node.h
  \brief Declaration file for the Node class

  This file is the declaration file for the Node class.

  \ingroup dnlElements
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlElements_Node_h__
#define __dnlElements_Node_h__

#include <List.h>
#include <Vec3D.h>

class NodalField;
class BoundaryCondition;
class Element;

/*!
  \class Node
  \brief Declaration of the Node class

  \ingroup dnlElements
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/
class Node
{
  friend class ListIndex<Node *>; //! To be able to use ListIndex
  long _listIndex;                //!< Local index used for the ListIndex management.

public:
  //  double initialTemperature;     //!< Initial Temperature. This field is used to store the reference value of the temperature of the node at the begining of the calculus
  double mass;                   //!< Mass. This field is used to store the value of the getJ2 nodal mass.
  long number;                   //!< Identification number. This field represents the external identification number of the current node (ie. user representation).
  NodalField *newField;     //!< New Nodal field of the node, ie. nodal field at the end of the current increment
  NodalField *currentField;        //!< Nodal field of the node, ie. nodal field at the begining of the current increment
  Vec3D coordinates;             //!< Coordinates of the corresponding node.
  //Vec3D initialCoordinates;      //!< Coordinates. Initial coordinates of the corresponding point.
  Vec3D normal;                  //!< Normal vector. This vector represents the normal vector of the current point in 3D space. Such normal vector is compted from the values of the normals of the faces connected to this point.
  BoundaryCondition *boundary;   //!< Boundary conditions. This pointer reference the list of the boundary conditions on the current node.
  ListIndex<Element *> elements; //!< Element list. This pointer reference the list of the elements that contains a reference to the current node (ie: the list of the elements that includes this node).
  //NodeMotion *motion;          //!< Node motion. This pointer reference the method used to move the point.

public:
  Node(long nodeNumber = 1, double xCoord = 0.0, double yCoord = 0.0, double zCoord = 0.0);
  Node(const Node &X);
  ~Node();

  /*   bool operator<(const Node &node) const;
  bool operator>(const Node &node) const;
 */
  bool operator!=(const Node &node) const;
  bool operator==(const Node &node) const;
  double getNodalValue(short field);
  long &internalNumber();
  long objectSize();
  NodalField *getNodalField(short time);
  SymTensor2 getNodalSymTensor(short field);
  Tensor2 getNodalTensor(short field);
  Vec3D getNodalVec3D(short field);
  void swapNodalFields();
  void copyNodalFieldToNew();

#ifndef SWIG
  friend std::ifstream &operator>>(std::ifstream &, Node &);
  friend std::ofstream &operator<<(std::ofstream &, const Node &);
  Node &read(std::ifstream &);
  friend std::ostream &operator<<(std::ostream &, Node &);
  void print(std::ostream &) const;
  void write(std::ofstream &) const;
#endif

  /**Attach an element. This method is used to add a new reference to an element in the list of the elements connected to the current point. There is no limit in the number of elements connected to the current node, so there is no verification procedure to see if this is correct for the structure.*/
  /*    
    
    
 */
  /**.Number of elements. This method returns the number of elements referenced in the list of the elements connected to the current node.*/
  //  long NoElements ();

  /***/
  //

  /**Node motion. This method is used to attach a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
  //   bool attachNodeMotion ( NodeMotion *motion );

  /**Node motion. This method is used to detach a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
  //   bool detachNodeMotion ();

  /**Node motion. This method is used to delete a new node motion control to the current node. See the class NodeMotion for more details about the definition of this motion control.*/
  //   bool deleteNodeMotion ();

  /**Nodal field transfer. This method is used to transfer the newField nodal field to the currentField one. In fact, this swaps the two fields.*/

  /**Get a nodal field. This method returns a pointer with a reference to one of the nodal fields depending on the given parameter time. Settings time=0 returns the currentField nodal field, time=1 returns the newField nodal field.*/

  // fonctions entree sortie
  //   void toFile ( FILE *pfile );
  //   void toFileBound ( FILE *pfile );
  //}
};

bool compareNodesNumber(Node *node1, Node *node2);
long substractNodesNumber(Node *node1, const long number);

/*
//-----------------------------------------------------------------------------
inline long Node::NoElements ()
//-----------------------------------------------------------------------------
{
    return elements.getSize ();
}
*/

//!Internal number. This method gives a direct access to the internal number of the current node.
/*!
  \return Internal number of the current node.
*/
//-----------------------------------------------------------------------------
inline long &Node::internalNumber()
//-----------------------------------------------------------------------------
{
  return (_listIndex);
}

//-----------------------------------------------------------------------------
inline NodalField *Node::getNodalField(short time)
//-----------------------------------------------------------------------------
{
  return (time == 0 ? currentField : newField);
}

#endif