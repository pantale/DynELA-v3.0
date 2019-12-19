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

#include <DynELA.h>
#include <Domain.h>
#include <Material.h>
#include <Node.h>
#include <Element.h>
#include <NodeSet.h>
#include <ElementSet.h>
#include <LogFile.h>
#include <ElQua4N2D.h>
#include <ElTri3N2D.h>
#include <ElQua4NAx.h>
#include <ElHex8N3D.h>
#include <ElTet4N3D.h>
#include <ElTet10N3D.h>
#include <ElementSet.h>
#include <BoundaryCondition.h>
#include <Boundary.h>
#include <Solver.h>
#include <VtkInterface.h>

/* #define nodeDisplayOnlineFrequency 100
#define elementDisplayOnlineFrequency 100
 */
DynELA *dynelaData = NULL; // initialisation par defaut sur NULL
/* extern String parsedFileName;
extern CPUrecord recordTimes;
 */
/*!
  \file DynELA.C
  \brief fichier .C de definition des structures elements finis
  \ingroup femLibrary

  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/

//!constructeur par defaut de la classe DynELA
/*!
  Cette methode est le constructeur par defaut de la classe DynELA. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
DynELA::DynELA(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;

  //  name = "defaultModel";

  // creates a domain and add it to the list of domains of DynELA
  domains << new Domain;

  // Creates a settings and add it
  settings = new Settings;

  // refer the global DynELA object
  if (dynelaData != NULL)
  {
    fatalError("DynELA::DynELA", "Only One DynELA object is allowed for a Finite Element program");
  }
  dynelaData = this;

  // on cree egalement un fichier log pour la lecture des donnees
  //  logFile = new LogFile(inter_name+".log");
  // logFile = new LogFile("DynELA.log");
  logFile.init("DynELA.log");

  // Creates a VTK interface for storing results
  dataFile = new VtkInterface;
  _resultFileName = name;

  // Creates and start a global timer
  cpuTimes.timer("Global")->start();

  // Creates the timers
  cpuTimes.add(new Timer("Global:Solver"));
  cpuTimes.add(new Timer("Solver:TimeStep"));
  cpuTimes.add(new Timer("Solver:Jacobian"));
  cpuTimes.add(new Timer("Solver:InternalForces"));
  cpuTimes.add(new Timer("Solver:ExplicitSolve"));
  cpuTimes.add(new Timer("Solver:Strains"));
  cpuTimes.add(new Timer("Solver:Density"));
  cpuTimes.add(new Timer("Solver:Predictor"));
  cpuTimes.add(new Timer("Solver:Pressure"));
  cpuTimes.add(new Timer("Solver:Stress"));
  cpuTimes.add(new Timer("Solver:FinalRotation"));

 /*  printf("Max Threads %d\n", omp_get_max_threads());
  omp_set_num_threads(1);
  printf("Num Threads %d\n", omp_get_max_threads());
 */
  /*  

  // creer les fichiers de sauvegarde
  resultFile=new io_Data;
  String inter_name=parsedFileName.before(sourceFileExtension);
  resultFile->binaryFile()=true;
  resultFile->link(inter_name);
  resultFile->link(this);

  nextSaveTime=0.0;
  previousSaveTime=-1;
  currentTime=0.0;
  _defaultElement=NULL; */
}

//!constructeur par recopie de la classe DynELA
/*!
  Cette methode est le constructeur par recopie de la classe DynELA. En pratique, ici, on ne fait rien de plus que les allocations de memoire necessaires.
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
DynELA::DynELA(const DynELA &X)
//-----------------------------------------------------------------------------
{
}

//!destructeur de la classe DynELA
/*!
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
DynELA::~DynELA()
//-----------------------------------------------------------------------------
{
  // on detruit le logFile
  logFile.close();

  // On sauve les logs de CPU
  std::string cpuFileName;
  dynelaData->settings->getValue("CpuFileName", cpuFileName);
  cpuTimes.saveReport(cpuFileName.c_str());
}

//!recherche d'un noeud dans la structure en fonction de son numero
/*!
  Cette methode recherche un noeud dans la structure en fonction de son numero et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. Le noeud est recherche sur la grille courante du domaine courant.
  \param nodeNumber numero du noeud e rechercher
  \return pointeur sur le noeud trouve ou NULL en cas d'echec de recherche
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
Node *DynELA::getNodeByNum(long nodeNumber)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (getCurrentDomain()->nodes.getSize() > 0)
  {
    if (getCurrentDomain()->nodes.last()->number == nodeNumber)
      return getCurrentDomain()->nodes.last();
  }

  // no so search for it
  return getCurrentDomain()->nodes.dichotomySearch(substractNodesNumber, nodeNumber);
}

//!recherche d'un element dans la structure en fonction de son numero
/*!
  Cette methode recherche un element dans la structure en fonction de son numero et renvoie un pointeur sur celui-ci, ou NULL si celui-ci n'existe pas dans la structure. L'element est recherche sur la grille courante du domaine courant.
  \param elementNumber numero de l'element e rechercher
  \return pointeur sur l'element trouve ou NULL en cas d'echec de recherche
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
Element *DynELA::getElementByNum(long elementNumber)
//-----------------------------------------------------------------------------
{
  // pehaps it's just the last one (often assumed)
  if (getCurrentDomain()->elements.getSize() > 0)
  {
    if (getCurrentDomain()->elements.last()->number == elementNumber)
      return getCurrentDomain()->elements.last();
  }

  // no so search for it
  return getCurrentDomain()->elements.dichotomySearch(substractElementsNumber, elementNumber);
}

//!creation d'un noeud et ajout e la structure
/*!
  Cette methode cree un nouveau noeud et l'ajoute e la fois e la liste des noeuds de la structure et e la liste des noeuds de la grille courante du domaine courant.
  \param nodeNumber numero du nouveau noeud e creer
  \param x coordonnee x du noeud e creer
  \param y coordonnee y du noeud e creer
  \param z coordonnee z du noeud e creer
  \version 1.0.0
  \date 2002
  \author Olivier PANTALE 
*/
//-----------------------------------------------------------------------------
bool DynELA::createNode(long nodeNumber, double xCoord, double yCoord, double zCoord)
//-----------------------------------------------------------------------------
{
  // pointeur sur le nouveau noeud
  Node *newNode = new Node(nodeNumber, xCoord, yCoord, zCoord);

  // l'ajouter e la grille courante du domaine courant
  domains.current()->add(newNode);

  // add the newNode and compact list
  nodes << newNode;
  nodes.compact();

  /*    if (nodes.getSize() % nodeDisplayOnlineFrequency == 0)
    displayOnline(); */

  // logFile
  char st[80];
  sprintf(st, "%ld [%lf,%lf,%lf]", newNode->number, newNode->coordinates(0), newNode->coordinates(1), newNode->coordinates(2));
  logFile << "Node : " << st << " added to " << name << "\n";

  return true;
}

//!renvoie le nombre de noeuds de la structure
/*!
  Cette methode renvoie le nombre total de noeuds de la structure

  \return nombre de noeuds de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
long DynELA::getNodesNumber()
//-----------------------------------------------------------------------------
{
  return nodes.getSize();
}

//-----------------------------------------------------------------------------
long DynELA::getElementsNumber()
//-----------------------------------------------------------------------------
{
  return elements.getSize();
}

//-----------------------------------------------------------------------------
bool DynELA::createElement(long elementNumber, long nodesIndex, ...)
//-----------------------------------------------------------------------------
{
  long nbNodes;
  long nNodes[50];

  // test if the default type is set
  if (_defaultElement == Element::Unknown)
  {
    fatalError("DynELA::createElement", "Element type not set\n");
  }

  // nouveau pointeur
  Element *pel = NULL;

  switch (_defaultElement)
  {
  case Element::ElQua4N2D:
    pel = new ElQua4N2D(elementNumber);
    break;
  case Element::ElTri3N2D:
    pel = new ElTri3N2D(elementNumber);
    break;
  case Element::ElQua4NAx:
    pel = new ElQua4NAx(elementNumber);
    break;
  case Element::ElHex8N3D:
    pel = new ElHex8N3D(elementNumber);
    break;
  case Element::ElTet4N3D:
    pel = new ElTet4N3D(elementNumber);
    break;
  case Element::ElTet10N3D:
    pel = new ElTet10N3D(elementNumber);
    break;
  default:
    fatalError("DynELA::createElement", "Unknown element type");
  }

  pel->createIntegrationPoints();
  nbNodes = pel->getNumberOfNodes();

  // A place to store the list of arguments
  va_list arguments;

  //Initializing arguments to store all values
  va_start(arguments, nodesIndex);
  nNodes[0] = nodesIndex;

  for (long i = 1; i < nbNodes; i++)
  {
    nNodes[i] = va_arg(arguments, long);
  }

  domains.current()->create(pel, nNodes);

  // add the element and compact list
  elements << pel;
  elements.compact();

  String str;
  logFile << "Element " << pel->getName() << " : " << str.convert(pel->number) << " [";
  for (long i = 0; i < nbNodes; i++)
  {
    if (i != 0)
      logFile << ", ";
    logFile << str.convert(pel->nodes(i)->number);
  }
  logFile << "] added to " << name << "\n";

  //Cleans up the list
  va_end(arguments);

  /*   if (elements.getSize() % elementDisplayOnlineFrequency == 0)
    displayOnline(); */

  return true;
}

//-----------------------------------------------------------------------------
void DynELA::setDefaultElement(short type)
//-----------------------------------------------------------------------------
{
  _defaultElement = type;
}

//-----------------------------------------------------------------------------
Domain *DynELA::getCurrentDomain()
//-----------------------------------------------------------------------------
{
  return domains.current();
}

//-----------------------------------------------------------------------------
void DynELA::add(Parallel *_parallel)
//-----------------------------------------------------------------------------
{
  parallel = _parallel;
}

//!affecte un materiau e un ensemble d'elements
/*!
  Cette methode affecte un materiau e un ensemble d'elements de la structure.

  \param material materiau e utiliser
  \param elementSet ElementSet e utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void DynELA::add(Material *material, ElementSet *elementSet)
//-----------------------------------------------------------------------------
{
  long i;
  bool already = false;

  // attach all elements of the group
  for (i = 0; i < elementSet->getSize(); i++)
  {
    elementSet->elements(i)->add(material);

    // logFile
    logFile << "Material " << material->name << " attached to element " << elementSet->elements(i)->number << "\n";
  }

  // attach the group to the material
  material->_elementsSet << elementSet;

  // attach the material to the structure if not already done
  for (i = 0; i < materials.getSize(); i++)
  {
    if (materials(i) == material)
    {
      already = true;
      i = materials.getSize();
    }
  }
  if (already == false)
    addMaterial(material);
}

//!ajoute un materiau e la structure
/*!
  Cette methode ajoute un nouveau materiau e la structure. Le nouveau materiau est initialise et renseigne de maniere externe. Cette methode est uniquement destinee e le stocker au niveau de la stucture. Il est ensuite possible d'y faire reference e partir poissonRatio nom de ce materiau. Cette methode verifie que lorsque l'on ajoute un materiau, si le nom est declare, un autre materiau portant le meme nom n'est pas deje present dans la liste des materiaux. Dans ce cas, une erreur est alors generee.

  \param pmat pointeur sur le nouveau materiau e ajouter e la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void DynELA::addMaterial(Material *pmat)
//-----------------------------------------------------------------------------
{
  long i;

  if (pmat->name != "")
  {
    for (i = 0; i < materials.getSize(); i++)
    {
      if (materials(i)->name == pmat->name)
      {
        fatalError("DynELA::addMaterial", "Material %s already exist\n", pmat->name.chars());
      }
    }
  }

  // ajout e la liste courante
  materials << pmat;

  // logFile
  logFile << "Material " << pmat->name << " added to " << name << "\n";
}

//-----------------------------------------------------------------------------
void DynELA::add(NodeSet *nodeSet, long startNumber, long endNumber, long increment)
//-----------------------------------------------------------------------------
{
  domains.current()->add(nodeSet, startNumber, endNumber, increment);
}

//-----------------------------------------------------------------------------
void DynELA::add(ElementSet *elementSet, long startNumber, long endNumber, long increment)
//-----------------------------------------------------------------------------
{
  domains.current()->add(elementSet, startNumber, endNumber, increment);
}

//!affecte des conditions aux limites initiales e un ensemble de noeuds
/*!
  Cette methode affecte des conditions aux limites e un ensemble de noeuds de la structure.

  \param boundary condition limite e utiliser
  \param nodeSet NodeSet e utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void DynELA::attachInitialBC(Boundary *boundary, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  BoundaryCondition *boundaryCondition;

  for (long i = 0; i < nodeSet->getSize(); i++)
  {

    // test if boundary exists already
    if ((boundaryCondition = nodeSet->nodes(i)->boundary) == NULL)
    {
      // no, we have to create it
      boundaryCondition = new BoundaryCondition;
      nodeSet->nodes(i)->boundary = boundaryCondition;
      logFile << "Boundary condition created for node " << nodeSet->nodes(i)->number << "\n";
    }

    // add the boundary condition
    boundaryCondition->addInitial(boundary);

    // logFile
    logFile << "Initial boundary condition attached to node " << nodeSet->nodes(i)->number << "\n";
  }

  // ajout du node set de reference
  boundary->nodeSet << nodeSet;
  /* bool tp = 0;
  boundary->typeSet << tp; */
}

//!affecte des conditions aux limites constantes e un ensemble de noeuds
/*!
  Cette methode affecte des conditions aux limites e un ensemble de noeuds de la structure.

  \param boundary condition limite e utiliser
  \param nodeSet NodeSet e utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void DynELA::attachConstantBC(Boundary *boundary, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  BoundaryCondition *boundaryCondition;

  for (long i = 0; i < nodeSet->getSize(); i++)
  {

    // test if boundary exists already
    if ((boundaryCondition = nodeSet->nodes(i)->boundary) == NULL)
    {
      // no, we have to create it
      boundaryCondition = new BoundaryCondition;
      nodeSet->nodes(i)->boundary = boundaryCondition;
      logFile << "Boundary condition created for node " << nodeSet->nodes(i)->number << "\n";
    }

    // add the boundary condition
    boundaryCondition->addConstant(boundary);

    // logFile
    logFile << "Constant boundary condition attached to node " << nodeSet->nodes(i)->number << "\n";
  }

  // ajout du node set de reference
  boundary->nodeSet << nodeSet;
  /* bool tp = 1;
  boundary->typeSet << tp; */
}

//-----------------------------------------------------------------------------
void DynELA::setSaveTimes(double startsavetime, double stopsavetime, double savetime)
//-----------------------------------------------------------------------------
{
  startSaveTime = startsavetime;
  endSaveTime = stopsavetime;
  saveTimeIncrement = savetime;
}

//-----------------------------------------------------------------------------
void DynELA::translate(Vec3D translateVector, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->getSize(); i++)
      nodeSet->nodes(i)->coordinates += translateVector;
  else
    for (long i = 0; i < nodes.getSize(); i++)
      nodes(i)->coordinates += translateVector;
}

//-----------------------------------------------------------------------------
void DynELA::scale(double scaleValue, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->getSize(); i++)
      nodeSet->nodes(i)->coordinates *= scaleValue;
  else
    for (long i = 0; i < nodes.getSize(); i++)
      nodes(i)->coordinates *= scaleValue;
}

//-----------------------------------------------------------------------------
void DynELA::scale(Vec3D scaleVector, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->getSize(); i++)
      for (long c = 0; c < 3; c++)
        nodeSet->nodes(i)->coordinates(c) *= scaleVector(c);
  else
    for (long i = 0; i < nodes.getSize(); i++)
      for (long c = 0; c < 3; c++)
        nodes(i)->coordinates(c) *= scaleVector(c);
}

//-----------------------------------------------------------------------------
void DynELA::rotate(String set, double angle, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  Tensor2 Mat;
  Mat = 0;

  double angleRadians = angle * dnlDegToRad;

  // rotation autour de X
  if (set == "X" || set == "x")
  {
    Mat(0, 0) = 1;
    Mat(1, 1) = cos(angleRadians);
    Mat(1, 2) = sin(angleRadians);
    Mat(2, 1) = -sin(angleRadians);
    Mat(2, 2) = cos(angleRadians);
  }

  // rotation autour de Y
  if (set == "Y" || set == "y")
  {
    Mat(0, 0) = cos(angleRadians);
    Mat(0, 2) = -sin(angleRadians);
    Mat(1, 1) = 1;
    Mat(2, 0) = sin(angleRadians);
    Mat(2, 2) = cos(angleRadians);
  }

  // rotation autour de Z
  if (set == "Z" || set == "z")
  {
    Mat(0, 0) = cos(-angleRadians);
    Mat(0, 1) = sin(-angleRadians);
    Mat(1, 0) = -sin(-angleRadians);
    Mat(1, 1) = cos(-angleRadians);
    Mat(2, 2) = 1;
  }

  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->getSize(); i++)
      nodeSet->nodes(i)->coordinates = Mat * nodeSet->nodes(i)->coordinates;
  else
    for (long i = 0; i < nodes.getSize(); i++)
      nodes(i)->coordinates = Mat * nodes(i)->coordinates;
}

//-----------------------------------------------------------------------------
void DynELA::rotate(Vec3D axis, double angle, NodeSet *nodeSet)
//-----------------------------------------------------------------------------
{
  Tensor2 Mat;

  double angleRadians = angle * dnlDegToRad;

  // normalisation de l'axe
  axis.normalize();

  Mat(0, 0) = dnlSquare(axis(0)) + cos(angleRadians) * (-dnlSquare(axis(0)) + 1);
  Mat(0, 1) = axis(0) * axis(1) - axis(2) * sin(angleRadians) - axis(0) * axis(1) * cos(angleRadians);
  Mat(0, 2) = axis(0) * axis(2) + axis(1) * sin(angleRadians) - axis(0) * axis(2) * cos(angleRadians);
  Mat(1, 0) = axis(0) * axis(1) + axis(2) * sin(angleRadians) - axis(0) * axis(1) * cos(angleRadians);
  Mat(1, 1) = dnlSquare(axis(1)) + cos(angleRadians) * (-dnlSquare(axis(1)) + 1);
  Mat(1, 2) = axis(1) * axis(2) - axis(0) * sin(angleRadians) - axis(1) * axis(2) * cos(angleRadians);
  Mat(2, 0) = axis(0) * axis(2) - axis(1) * sin(angleRadians) - axis(0) * axis(2) * cos(angleRadians);
  Mat(2, 1) = axis(1) * axis(2) + axis(0) * sin(angleRadians) - axis(1) * axis(2) * cos(angleRadians);
  Mat(2, 2) = dnlSquare(axis(2)) + cos(angleRadians) * (-dnlSquare(axis(2)) + 1);

  if (nodeSet != NULL)
    for (long i = 0; i < nodeSet->getSize(); i++)
      nodeSet->nodes(i)->coordinates = Mat * nodeSet->nodes(i)->coordinates;
  else
    for (long i = 0; i < nodes.getSize(); i++)
      nodes(i)->coordinates = Mat * nodes(i)->coordinates;
}

//!calcule les coordonnees mini et maxi de l'ensemble des noeuds d'une structure
/*!
  Cette methode calcule l'enveloppe mini et maxi des coordonnees des points d'uns structure complete

  \param min coordonnee minimale
  \param max coordonnee maximale
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void DynELA::getGlobalBox(Vec3D &minPoint, Vec3D &maxPoint)
//-----------------------------------------------------------------------------
{
  Vec3D coordinates;
  long i, j;

  // affectation par defaut au commencement
  maxPoint = minPoint = nodes(0)->coordinates;

  // boucle de recherche
  for (i = 1; i < nodes.getSize(); i++)
  {
    coordinates = nodes(i)->coordinates;

    for (j = 0; j < 3; j++)
    {
      if (coordinates(j) < minPoint(j))
        minPoint(j) = coordinates(j);
      if (coordinates(j) > maxPoint(j))
        maxPoint(j) = coordinates(j);
    }
  }
}

//-----------------------------------------------------------------------------
void DynELA::writeResultFile()
//-----------------------------------------------------------------------------
{
  String fileName;
  String number;
  number.convert(_resultFileIndex, 3);
  fileName = _resultFileName + number + ".vtk";

  // Initialize the vtk data file
  dataFile->init(fileName);
  // Write the vtk data file
  dataFile->write();
  // Close the vtk data file
  dataFile->close();

  logFile << "Result file: " << fileName << " written at time " << currentTime << " s\n";

  // increment the index
  _resultFileIndex++;
}

//!lancement du solveur general
/*!
  Cette methode lance la procedure de solveur general de la structure. Elle prend en compte tous les types de solveurs possible et gere aussi bien la resolution mono-domaine que la resolution multi-domaine. C'est le point d'entree de tout solveur.

  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void DynELA::solve()
//-----------------------------------------------------------------------------
{
  cpuTimes.timer("Solver")->start();

  bool solved = false;

  // affichage
  std::cout << "\nProcessing DynELA ...\n";
  logFile.separatorWrite("DynELA Solver Initialization phase");

  // Run the init solve of this domain
  for (short domainId = 0; domainId < domains.getSize(); domainId++)
  {
    domains(domainId)->initSolve();
  }

  // Get the end time of the structure
  double endOfComputationTime = domains(0)->getEndSolveTime();
  for (short domainId = 1; domainId < domains.getSize(); domainId++)
  {
    endOfComputationTime = dnlMin(domains(domainId)->getEndSolveTime(), endOfComputationTime);
  }
  logFile << "Set final computation time to: " << endOfComputationTime << " s\n";

  // Save initial configuration
  writeResultFile();

  // Next save time
  nextSaveTime = startSaveTime + saveTimeIncrement;

  logFile.separatorWrite("DynELA Solver phase");

  // Only one domain, so that's simple, only a direct solve
  if (domains.getSize() == 1)
  {
    // do the loops until the end of the computation for this domain
    while (nextSaveTime <= endOfComputationTime)
    {
      // Run the solver for the current domain
      solved = domains(0)->solve(nextSaveTime);

      // Test if solved
      if (solved == false)
      {
        writeResultFile();
        fatalError("Solver Error", "Unable to solve problem upto time = %10.3E\n", nextSaveTime);
      }

      // Update the current time of the whole structure
      currentTime = domains(0)->currentTime;

      // Write a new result file
      writeResultFile();

      // Computes the next saveTime value
      if ((nextSaveTime + saveTimeIncrement > endOfComputationTime) && (nextSaveTime < endOfComputationTime))
        nextSaveTime = endOfComputationTime;
      else
        nextSaveTime += saveTimeIncrement;
    }
  }
  else
  {
    fatalError("DynELA::solve", "Multi-domains solve not implemented yet\n");
  }

  cpuTimes.timer("Solver")->stop();

  // On affiche les logs de CPU
  cpuTimes.stop();

  /*  
   long i;
   nextTime;
  bool cont;
  double endOfComputationTime;

  nextTime = 0;

  else
  {
    cont = true;
    while (cont)
    {
      cont = false;

      // what is the next time
      nextTime = currentTime + saveTimeIncrement / 500;

      for (i = 0; i < domains.getSize(); i++)
      {
        if (domains(i)->solve(nextTime) == true)
          cont = true;
      }

      // Synchronize times
      currentTime = dnlMin(domains(0)->currentTime, domains(0)->currentTime);

      saveResults();
    }
  }

  // final result if available
  saveResults();

  cout << "end of solve\n";
  // print the CPU times
  recordTimes.report("CPU-TIMES"); */
}

/*

//!Initialise les structures memoire pour la resolution
/*!
  Cette methode est utilisee pour initialiser les structures memoire apres lecture des donnees dans le fichier source. C'est la phase de preprocessing de la structure. Un ensemble de verifications sont effectuees dans cette methode.

  \return true si la methode n'a genere aucune erreur
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
bool DynELA::initSolve ()
//-----------------------------------------------------------------------------
{
  long i;

  cout << "\nPreProcessing of the structure ...\n";

  // log file
  logFile->separatorWrite();
  logFile << "\nInitialisation of solver phase ........\n\nStructure architecture\n";

  // log file
  logFile << elements.getSize()<<" elements\n"<<nodes.getSize()<<" nodes\n";

  // initialisation du temps
  currentTime=0;
	// initialisation des domaines
  for (i=0; i<domains.getSize();i++) 
    {
      logFile << "\nVerification of domain "<<i<<" ...\n";
      if (domains(i)->initSolve()==false)
	{
	  // domain has to be deleted
	  logFile << "Void Domain "<<i<< " has been deleted ...\n";
	  delete domains(i);
	  domains.del(i);
	  i--;
	}
    }
  logFile << "\nPhysic contains "<<i<< " domain"<<(i>1 ? "s":"")<<" ...\n\n";

  // si pas de domaines, alors pas de physique

  // si pas de dommaines, alors pas de structure
  if (domains.getSize()==0) return (false);

  // verification de la coherence des materiaux
  for (i = 0; i < materials.getSize (); i++)
    {
      logFile << "Verification of material "<<i<<" named "<<materials (i)->name<<" ...\n";
      materials (i)->checkValues ();
    }

  // calcul des materiaux 
  for (i = 0; i < materials.getSize (); i++)
    {
      logFile << "Initialisation of material "<<i<<" named "<<materials (i)->name<<" ...\n";
      materials (i)->computeHookeTensor ();
    }

  logFile << "\nEnd of initialisation of solver phase ........\n";

  // preparation des save times
  if (startSaveTime == 0) startSaveTime=saveTimeIncrement; 
  nextSaveTime = startSaveTime;

  nodes.sort();
  elements.sort();

  return (true);
}

//!recherche si un materiau existe actuellement
/*!
  Cette methode est utilisee pour recuperer un materiau donne e partir de son nom dans une structure. Elle cherche un materiau donne en fonction de son nom et renvoie un pointeur sur ce materiau (s'il existe) ou NULL dans le cas oe le materiau n'a pu etre trouve dans la liste des materiaux de la structure.

  \param name nom du materiau e recuperer
  \return un pointeur sur ce materiau (s'il existe) ou NULL dans le cas oe le materiau n'a pu etre trouve dans la liste des materiaux de la structure.
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
Material * DynELA::getMaterial (String name)
//-----------------------------------------------------------------------------
{
  // balayage de la liste des materiaux
  for (long i = 0; i < materials.getSize (); i++)
    {
      // materiau trouve
      if (materials (i)->name == name)
	return materials (i);
    }

  // materiau non trouve
  return NULL;
}

//!affecte des conditions aux limites e un ensemble de noeuds
/*!
  Cette methode affecte des conditions aux limites e un ensemble de noeuds de la structure.

  \param BC condition limite e utiliser
  \param nds NodeSet e utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
void DynELA::attachBCToNodes(BoundaryCondition* BC, NodeSet* nds)
//-----------------------------------------------------------------------------
{
  for (long i=0;i<nds->getSize();i++){
    nds->nodes(i)->boundary=BC;

  // logFile
   logFile << "A boundary condition has been attached to node "<<nds->nodes(i)->number<<"\n";
  }
}

//!ajoute un solveur e la structure
/*!
  Cette methode ajoute un solveur e la structure. Le solveur est ajoute e la liste des solveurs du domaine courant de la structure.

  \param solver solveur e utiliser
  \author Olivier PANTALE
  \since DynELA 1.0.0





//!renvoie le nombre d'elements de la structure
/*!
  Cette methode renvoie le nombre total d'elements de la structure

  \return nombre d'elements de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0


//!renvoie le ieme noeud de la structure
/*!
  Cette methode renvoie le ieme noeud de la structure

  \param i indice du noeud
  \return ieme noeud de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
Node* DynELA::getNode(long i)
//-----------------------------------------------------------------------------
{
  return nodes(i);
}

//!renvoie le ieme element de la structure
/*!
  Cette methode renvoie le renvoie le ieme element de la structure

  \param i indice de l'element
  \return ieme element de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
Element* DynELA::getElement(long i)
//-----------------------------------------------------------------------------
{
  return elements(i);
}

//!renvoie un noeud en fonction de son Id
/*!
  Cette methode renvoie un noeud en fonction de son Id

  \param i Id du noeud
  \return ieme noeud de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
Node* DynELA::getNodeById(long i)
//-----------------------------------------------------------------------------
{
  return nodes.AppN(i);
}

//!renvoie un element en fonction de son Id
/*!
  Cette methode renvoie un element en fonction de son Id

  \param i Id de l'element
  \return ieme noeud de la structure
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
Element* DynELA::getElementById(long i)
//-----------------------------------------------------------------------------
{
  return elements.AppN(i);
}

//!selectionne un domaine dans la structure
/*!
  Cette methode permet de creer un nouveau domaine dans la structure ou de selectionner un autre domaine pour le domaine courant de la structure.

  \param domain pointeur sur le domaine e selectionner
  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
void DynELA::setDomain(Domain* domain)
//-----------------------------------------------------------------------------
{
//  long ind;
  // si le domaine existe deja, on n'a pas e le creer
  if (!domains.contains(domain)) 
    {
      domains << domain;
    }

  // change the current index
  domains(domains.getIndex(domain));
  logFile << "DynELA: "<<name << " domain: "<<domains.current()->name<<" selected\n";
}


//!affiche pendant la lecture des donnees un etat d'avancement sur la console
/*!
  Cette methode affiche pendant la lecture des donnees un etat d'avancement sur la console du remplissage memoire concernant les nombres de noeuds, elements et domaines de la structure complete.

  \author Olivier PANTALE
  \since DynELA 1.0.0

//-----------------------------------------------------------------------------
void DynELA::displayOnline()
//-----------------------------------------------------------------------------
{
//  long doms=0;

  // memoire du domaine
//  long indDom=domains.getIndex(domains.current());
  
//  for (long i=0;i<domains.getSize();i++) doms+=physics(i)->domains.getSize();

  printf("\rRead %ld domain%s %ld node%s %ld element%s %ld material%s",
	 domains.getSize(),(domains.getSize() > 1 ? "s" : ""),
	 nodes.getSize(),(nodes.getSize()>1 ? "s" : ""),
	 elements.getSize(),(elements.getSize()>1 ? "s" : ""),
	 materials.getSize(),(materials.getSize()>1 ? "s" : ""));

  // reset currentDomain
 // physics(indPhy);

  fflush(stdout);
}

//-----------------------------------------------------------------------------
void DynELA::saveResults()
//-----------------------------------------------------------------------------
{
  if ((currentTime>=nextSaveTime) && (currentTime!=previousSaveTime)) {

    // transfer the datas
    resultFile->transfert();

    // increment saveTimes
    previousSaveTime=currentTime;
    nextSaveTime+=saveTimeIncrement;
  }
}


//-----------------------------------------------------------------------------
double DynELA::getReadTimeData (ifstream & pfile)
//-----------------------------------------------------------------------------
{
  double tim;

  if (checkBinaryVersion (pfile, 0) != Ok)
    fatalError ("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read ((char *) &tim, sizeof (double));

  return tim;
}

//-----------------------------------------------------------------------------
void DynELA::readData (ifstream & pfile)
//-----------------------------------------------------------------------------
{
  long i;
  //  long currentDomain;

  if (checkBinaryVersion (pfile, 0) != Ok)
    fatalError ("Major changes done since this old version", "Sorry !! ...");

  // load current time
  pfile.read ((char *) &currentTime, sizeof (double));

  // load current domain number
  //  pfile.read ((char *) &currentDomain, sizeof (long));
  //  domains(currentDomain);

  // load the domains
  for (i = 0; i < domains.getSize (); i++)
    {
      domains(i)->readData(pfile);
      if (checkBinaryVersion (pfile, 1) != Ok)
	fatalError ("domain datas", "Read error");
    }
}

//-----------------------------------------------------------------------------
void DynELA::writeData (ofstream & pfile)
//-----------------------------------------------------------------------------
{
  long i;
  // long currentPhysic;

  checkBinaryVersionWrite (pfile, 0);

  // save current time
  pfile.write ((char *) &currentTime, sizeof (double));

  // load the domains
  for (i = 0; i < domains.getSize (); i++)
    {
      domains(i)->writeData(pfile);
      checkBinaryVersionWrite (pfile, 1);
    }
}

//-----------------------------------------------------------------------------
bool DynELA::checkBinaryVersion (ifstream & pfile, int top)
//-----------------------------------------------------------------------------
{
  Vec3D version, check;

  if (top == 0)
    {
      pfile >> version;
      pfile >> check;

      if (check (0) != versionCheckDefine)
	{
	  cerr << "File recorded by DynELA v." << (int) version (0) << "." <<
	    (int) version (1) << "." << (int) version (2) << "-" << (int)
	    check (1) << endl;
	  cerr << endl << "This recording format is not compatible with ";
	  cerr << "DynELA v." << VERSION << "." << RELEASE << "." <<
	    UNDER_RELEASE << "-" << ALPHA_VERSION << endl;
	  cerr << "\nYou MUST delete previous results and rerun the solver\n";
	  return Fail;
	}
      return Ok;
    }
  else
    {
      pfile >> check;
      if (check (0) != separatorCheckDefine)
	{
	  cerr << "\n May be file is trucated\n";
	  return Fail;
	}
      return Ok;
    }
}

//-----------------------------------------------------------------------------
void DynELA::checkBinaryVersionWrite (ofstream & pfile, int top)
//-----------------------------------------------------------------------------
{
  Vec3D version, check;

  version (0) = (double) VERSION;
  version (1) = (double) RELEASE;
  version (2) = (double) UNDER_RELEASE;
  check (0) = versionCheckDefine;
  check (1) = ALPHA_VERSION;

  if (top == 0)
    {
      check (0) = versionCheckDefine;
      pfile << version;
      pfile << check;
    }
  else
    {
      check (0) = separatorCheckDefine;
      pfile << check;
    }
}


//-----------------------------------------------------------------------------
void DynELA::sortElementsAndNodes()
//-----------------------------------------------------------------------------
{
  nodes.compact();
  elements.compact();
}

//-----------------------------------------------------------------------------
void DynELA::setDefaultElement (Element * pel) 
//-----------------------------------------------------------------------------
{
  _defaultElement=pel;
}
 
//-----------------------------------------------------------------------------
void DynELA::addInterface (Interface* pinter)
//-----------------------------------------------------------------------------
{
#ifdef VERIF_assert
  assert (pinter!=NULL);
#endif

  domains.current()->addInterface(pinter);
  logFile << "Interface : " << pinter->name << " added to "<<name<<"\n";
}


//-----------------------------------------------------------------------------
void DynELA::compact()
//-----------------------------------------------------------------------------
{
  long i,j,k,l;
  Domain *pdomain;
//  Physic* pphysic;
//  Grid* pgrid;
  long elementNum,nodeNum;

//  for (i=0;i<physics.getSize();i++)
//    {
//      pphysic=physics(i);
      for (j=0;j<domains.getSize();j++)
	{
	  pdomain=domains(j);
//	  for(k=0;k<pdomain->grids.getSize();k++)
//	    {
	      elementNum=1;
	      nodeNum=1;
//	      pgrid=pdomain->grids(k);
	      for (l=0;l<pdomain->nodes.getSize();l++)
		{
		  pdomain->nodes(l)->number=nodeNum++;
		}
	      for (l=0;l<pdomain->elements.getSize();l++)
		{
		  pdomain->elements(l)->number=elementNum++;
		}
	    }
	//}
   // }
}

//-----------------------------------------------------------------------------
void DynELA::readResultFile(long num)
//-----------------------------------------------------------------------------
{
  // get the old mode
  char oldMode=resultFile->getMode();

  // set mode to read mode
  resultFile->setMode(Read);
  
  // set the increment
  resultFile->incrementalFile()=true;
  resultFile->startCurrentSave(num);

  // get the transfert
  resultFile->transfert();

  // rest the mode
  resultFile->setMode(oldMode);

  // logFile
  logFile << "Result datafile "<<resultFile->getFileName() << " loaded\n";
}

//-----------------------------------------------------------------------------
double DynELA::getCurrentTime()
//-----------------------------------------------------------------------------
{
  return currentTime;
}

//-----------------------------------------------------------------------------
void DynELA::getDataFileBounds(long &min, long& max)
//-----------------------------------------------------------------------------
{
  dirent *file;
  DIR* direct;
  direct= opendir(".");
  String sub;
  bool v0;
  long val;
  String filename=resultFile->getFileName();

  // skip file extension if exists
  if (filename.contains(sourceFileExtension)) filename=filename.before(sourceFileExtension);

  v0=false;
  bool ok=false;

  if ( !direct ) {
    fatalError("readDirectory","can't read directory");  
  }
  while ( (file = readdir(direct)) ) {
    String name(file->d_name);
    if ((name.contains(filename)) && (name.contains(resultFileExtension))) {     
      sub=name.before(resultFileExtension);
      sub=sub.after(filename);
      sub=sub.after(".");
      val=sub.getInt();
      if (!v0) {min=max=val;v0=true;}
      if (val<min) min=val;
      if (val>max) max=val;
      ok=true;
    }
  }
  if (!ok) min=max=0;
}

//-----------------------------------------------------------------------------
long DynELA::getLastDataFileNumber()
//-----------------------------------------------------------------------------
{
  long min,max;
  getDataFileBounds(min, max);
  return max;
}

//-----------------------------------------------------------------------------
long DynELA::getFirstDataFileNumber()
//-----------------------------------------------------------------------------
{
  long min,max;
  getDataFileBounds(min, max);
  return min;
}

//-----------------------------------------------------------------------------
double DynELA::getNodalValue(long nod,String field,long component)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->getNodalValue( field, component);
}

//-----------------------------------------------------------------------------
Vec3D DynELA::getNodalVector(long nod,String field)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->getNodalVector( field);
}

//-----------------------------------------------------------------------------
Tensor2 DynELA::getNodalTensor(long nod,String field)
//-----------------------------------------------------------------------------
{
  return getNodeByNum(nod)->getNodalTensor( field);
}

//-----------------------------------------------------------------------------
void DynELA::setResultFile(String file)
//-----------------------------------------------------------------------------
{
  resultFile->getFileName()=file;
}

//-----------------------------------------------------------------------------
void DynELA::mergeDomains()
//-----------------------------------------------------------------------------
{
  // if only one grid;
  if (domains.getSize()==1) return;

  long nnum=1,elnum=1;
  long i,j;
  Domain* pdomaind, *pdomaino;

  pdomaind=domains(0);

  for (j=0;j<pdomaind->elements.getSize();j++)
    {	  
      pdomaind->elements(j)->number=elnum++;
    }
  for (j=0;j<pdomaind->nodes.getSize();j++)
    {	  
      pdomaind->nodes(j)->number=nnum++;
    }

  for (i=1; i<domains.getSize();i++)
    {
      pdomaino=domains(i);

      // copy elements
      for (j=0;j<pdomaino->elements.getSize();j++)
	{	  
	  pdomaind->elements << pdomaino->elements(j);
	  pdomaino->elements(j)->number=elnum++;
	}

      // copy nodes
      for (j=0;j<pdomaino->nodes.getSize();j++)
	{	  
	  pdomaind->nodes << pdomaino->nodes(j);
	  pdomaino->nodes(j)->number=nnum++;
	}
    }

  domains.del(1,domains.getSize()-1);
}
*/