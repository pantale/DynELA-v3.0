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

#ifndef __dnlFEM_DynELA_h__
#define __dnlFEM_DynELA_h__

#include <Node.h>
#include <Element.h>
#include <LogFile.h>
#include <Settings.h>
#include <Timer.h>

#include <omp.h>

class DynELA;
#ifndef SWIG
extern DynELA *dynelaData;
#endif

/* class ElementSet;

#define versionCheckDefine 1.02
#define separatorCheckDefine 69
 */
/*!
  \file DynELA.h
  \brief fichier .h de definition des grilles elements finis
  \ingroup femSolver

  Ce fichier sert e la definition de la classe DynELA.

  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
class Model;
class Node;
class Element;
class LogFile;
class Material;
class NodeSet;
class ElementSet;
class VtkInterface;
class Boundary;
class Solver;
class Parallel;
class HistoryFile;

/* class Element;
class Node;
class io_Structure;
class Interface;
class HistoryFile;
class BoundaryCondition;
class NodeSet;
 */
//#include <io_Structure.h>

/*!
  \class DynELA DynELA.h
  \brief Classe de definition et de manipulation des structures elements finis.
  \ingroup femLibrary


  \author Olivier PANTALE
  \version 1.0.0
  \date 1997-2004
*/
class DynELA
{
  friend class VtkInterface;

private:
  short _defaultElement = Element::Unknown; //!< Current default Element
  short _resultFileIndex = 0;               //!< Current result file index
  String _resultFileName;                   //!< Current result file name

protected:
  ListIndex<Node *> nodes;       //!< Nodes list of the structure
  ListIndex<Element *> elements; //!< Elements list of the structure
  //List<Model *> models;          //!< Models list of the structure
  List<Material *> materials;    //!< Materials list of the structure

public:
  double endSaveTime = 0.0;       //!< Final save time
  double saveTimeIncrement = 0.0; //!< Increment of save time
  double nextSaveTime = 0.0;      //!< Next save time
  double startSaveTime = 0.0;     //!< Start save time
  double currentTime = 0.0;       //!< Current time
  String name = "_noname_";       //!< name of the object
  Settings *settings = NULL;      //!< Settings
  VtkInterface *dataFile = NULL;  //!< Interface for results
  Parallel *parallel = NULL;      //!< Parallel computation
  Model *model = NULL;            //!< Pointer to the model
  Timers cpuTimes;                //!< Store the CPU Times

#ifndef SWIG
  LogFile logFile; //!< Log file
#endif

public:
  DynELA(char *newName = NULL);
  DynELA(const DynELA &X);
  ~DynELA();

  bool createElement(long elementNumber, long nodesIndex, ...);
  bool createNode(long nodeNumber, double xCoord, double yCoord, double zCoord);
 // Model *model;
  Element *getElementByNum(long elementNumber);
  long getElementsNumber();
  long getNodesNumber();
  Node *getNodeByNum(long nodeNumber);
  void add(ElementSet *elementSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(Material *material, ElementSet *elementSet);
  void add(NodeSet *nodeSet, long startNumber = -1, long endNumber = -1, long increment = 1);
  void add(Parallel *parallel);
  void add(HistoryFile *newHistoryFile);
  void add(Solver *newSolver);
  void addMaterial(Material *pmat);
  void attachConstantBC(Boundary *boundary, NodeSet *nodeSet);
  void attachInitialBC(Boundary *boundary, NodeSet *nodeSet);
  void getGlobalBox(Vec3D &minPoint, Vec3D &maxPoint);
  void rotate(String set, double angle, NodeSet *nodeSet = NULL);
  void rotate(Vec3D axis, double angle, NodeSet *nodeSet = NULL);
  void scale(double scaleValue, NodeSet *nodeSet = NULL);
  void scale(Vec3D scaleVector, NodeSet *nodeSet = NULL);
  void setDefaultElement(short type);
  void setSaveTimes(double startsavetime, double stopsavetime, double savetime);
  void solve();
  void translate(Vec3D translateVector, NodeSet *nodeSet = NULL);
  void writeResultFile();

  //  void displayOnline();

  // member functions
  /*  
  bool checkBinaryVersion (ifstream & pfile, int);
  bool initSolve ();
  double getCurrentTime();
  double getNodalValue(long nod,String field,long component=0);
  double getReadTimeData (ifstream & pfile);
  Element* getElement (long num);
  Element* getElementById (long num);
  long getFirstDataFileNumber();
  long getLastDataFileNumber();
  Material* getMaterial (String name);
  Node* getNode (long num);
  Node* getNodeById (long num);
  Tensor2 getNodalTensor(long nod,String field);
  Vec3D getNodalVector(long nod,String field);
  void addInterface(Interface* pinter);
  void attachBCToNodes(BoundaryCondition* BC, NodeSet* nds);
  void checkBinaryVersionWrite (ofstream & pfile, int);
  void compact();
  void getDataFileBounds(long &min, long& max);
  void getGlobalBox (Vec3D & min, Vec3D & max);
  void mergeModels();
  void readData (ifstream & pfile);
  void readResultFile(long);
  void saveResults();
  void setDefaultElement (Element * pel);
  void setModel(Model* model);
  void setResultFile(String);
  void sortElementsAndNodes();
  void writeData (ofstream & pfile);
   */
};

#endif
