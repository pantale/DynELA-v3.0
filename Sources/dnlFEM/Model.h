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

#ifndef __dnlFEM_Model_h__
#define __dnlFEM_Model_h__

#include <String.h>
#include <List.h>
#include <DynELA.h>

class Node;
class Element;
class NodeSet;
class ElementSet;
class HistoryFile;

/*!
  \class Model Model.h
  \brief Classe de gestion et manipulation des modeles elements finis.
  \ingroup femLibrary


  \author Olivier PANTALE
  \version 0.9.4
  \date 1997-2004
*/
class Model
{
  friend class DynELA;

private:
  bool _massMatrixComputed = false; //!< Flag indiquant que la matrice de masse est deja calculee
  short _numberOfDimensions = 0;    //!< Number of dimensions of the model
  double _lastElapsedTime = 0;      //!< Last elapsed time for computing endtime
  double _lastElapsedComputeTime = 0;

  /*   
  Vector EV_TimeStep;
  double fmax0;
 */
public:
  double currentTime = 0.0;         //!< Temps actuel du modele
                                    // double nextTime = 0.0;           //!< Temps actuel du modele + increment de temps
  List<Element *> elements;         //!< List of the Elements
  List<ElementSet *> elementsSets;  //!< List of the Elements Sets
  List<Node *> nodes;               //!< List of the Nodes
  List<NodeSet *> nodesSets;        //!< List of the Nodes Sets
  List<HistoryFile *> historyFiles; //!< List of the History Files
                                    //  List<Solver *> solvers;          //!<liste des solveurs associes au modele
  Solver *solver = NULL;            //!< solveurs associes au modele
  MatrixDiag massMatrix;            //!< Mass matrix
                                    //  Solver *currentSolver = NULL;    //!< Current solver
  String name = "_noname_";         //!< Name of the model
  Vector internalForces;            //!< Vecteur des forces internes
  /*   
  List < Interface * > interfaces;          //!< Liste des interfaces de contact du modele
  FILE* history_file;                       //!< Fichier historique associe au modele
  long dimension; //!< Nombre de dimensions de la grille 
  */

private:
  bool add(Element *pel);
  bool add(Node *pnd);
  void add(ElementSet *elementSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(HistoryFile *newHistoryFile);
  void add(NodeSet *nodeSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(Solver *newSolver);

public:
  // constructeurs
  Model(char *newName = NULL);
  Model(const Model &model);
  ~Model();

  bool checkTopology();
  bool initSolve();
  bool solve(double solveUpToTime = -1.0);
  double computeCourantTimeStep();
  double getEndSolveTime();
  double getTotalKineticEnergy();
  double getTotalMass();
  Element *getElementByNum(long elementNumber);
  Node *getNodeByNum(long nodeNumber);
  short getNumberOfDimensions();
  void computeFinalRotation();
  void computeInternalForces();
  void computeJacobian();
  void computeMassMatrix(bool forceComputation = false);
  void computePressure();
  void computeStrains();
  void computeStress(double timeStep);
  void create(Element *pel, long *listOfNodesNumber);
  void transfertQuantities();
  void writeHistoryFiles();
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
  friend ostream & operator << (ostream & os, Model & dom);
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
inline short Model::getNumberOfDimensions()
//-----------------------------------------------------------------------------
{
  return _numberOfDimensions;
}

#endif
