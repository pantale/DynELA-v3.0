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

#ifndef __dnlFEM_Domain_h__
#define __dnlFEM_Domain_h__

#include <String.h>
#include <List.h>
#include <DynELA.h>

class Node;
class Element;
class NodeSet;
class ElementSet;
class HistoryFile;

/*!
  \class Domain Domain.h
  \brief Classe de gestion et manipulation des domaines elements finis.
  \ingroup femLibrary


  \author Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/
class Domain
{

private:
  bool _massMatrixComputed = false; //!< Flag indiquant que la matrice de masse est deja calculee
  short _numberOfDimensions = 0;    //!< Number of dimensions of the domain
  double _lastElapsedTime = 0;      //!< Last elapsed time for computing endtime
  double _lastElapsedComputeTime = 0;

  /*   
  Vector EV_TimeStep;
  double fmax0;
 */
public:
  double currentTime = 0.0;         //!< Temps actuel du domaine
                                    // double nextTime = 0.0;           //!< Temps actuel du domaine + increment de temps
  List<Element *> elements;         //!< List of the Elements
//  List<Element *> elementsCore[10]; //!< List of the Elements
  List<ElementSet *> elementsSets;  //!< List of the Elements Sets
  List<Node *> nodes;               //!< List of the Nodes
  List<NodeSet *> nodesSets;        //!< List of the Nodes Sets
  List<HistoryFile *> historyFiles; //!< List of the History Files
                                    //  List<Solver *> solvers;          //!<liste des solveurs associes au domaine
  Solver *solver = NULL;            //!< solveurs associes au domaine
  MatrixDiag massMatrix;            //!< Mass matrix
                                    //  Solver *currentSolver = NULL;    //!< Current solver
  String name = "_noname_";         //!< Name of the domain
  Vector internalForces;            //!< Vecteur des forces internes
  /*   
  List < Interface * > interfaces;          //!< Liste des interfaces de contact du domaine
  FILE* history_file;                       //!< Fichier historique associe au domaine
  long dimension; //!< Nombre de dimensions de la grille 
  */

  // constructeurs
  Domain(char *newName = NULL);
  Domain(const Domain &domain);
  ~Domain();

  bool add(Element *pel);
  bool add(Node *pnd);
  bool checkTopology();
  bool initSolve();
  bool solve(double solveUpToTime = -1.0);
  double computeCourantTimeStep();
  double getEndSolveTime();
  double getTotalKineticEnergy();
  double getTotalMass();
  Element *getElementByNum(long elementNumber);
  Node *getNodeByNum(long nodeNumber);
  void add(ElementSet *elementSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(NodeSet *nodeSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(Solver *newSolver);
  void computeInternalForces();
  void computeJacobian();
  void computeMassMatrix(bool forceComputation = false);
  void create(Element *pel, long *listOfNodesNumber);
  short getNumberOfDimensions();
  void transfertQuantities();
  void writeHistoryFiles();
  void add(HistoryFile *newHistoryFile);
  void computeStrains();
  void computePressure();
  void computeStress(double timeStep);
  void computeFinalRotation();

  /* 
  
  double computePowerIterationTimeStep();

  // gestion du temps
  double getTime ();
  void updateTime ();

  // interfaces
  void addInterface(Interface* inter);

  // fonctions de calculs

  void starterWrite (String name);
  double getCurrentTime();
  double getTimeStep();
  void getGlobalBox (Vec3D & min, Vec3D & max);

  // fonctions entree sortie
  void print (ostream & os) const;
  friend ostream & operator << (ostream & os, Domain & dom);
  void readData (ifstream & pfile);
  double getReadTimeData (ifstream & pfile);
  void writeData (ofstream & pfile);

  // methodes liees e la lecture des fichiers
  void createNode(long,double,double,double);
  Node* getNodeByNumber(long);
  //void setGrid(Grid* grid);

  // direct implementation
  long getNumberOfDimensions() {return dimension;} //!< Dimension topologique de la grille
  */
};

//-----------------------------------------------------------------------------
inline short Domain::getNumberOfDimensions()
//-----------------------------------------------------------------------------
{
  return _numberOfDimensions;
}

#endif
