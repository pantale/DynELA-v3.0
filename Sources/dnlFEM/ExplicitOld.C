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

#include <ExplicitOld.h>
#include <Node.h>
#include <DynELA.h>
#include <Model.h>
#include <NodalField.h>
#include <Boundary.h>
#include <BoundaryCondition.h>

extern DynELA *dynelaData;

//-----------------------------------------------------------------------------
ExplicitOld::ExplicitOld(char *newName) : Solver(newName)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ExplicitOld::ExplicitOld(const ExplicitOld &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ExplicitOld::~ExplicitOld()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void ExplicitOld::computePredictions()
//-----------------------------------------------------------------------------
{
  long nodeId;
  Node *node;

  double alpha = 0.;
  double dzeta = 0.;
  double gamma = 0.;
  double beta = 0.;

  // phase de prediction
  for (nodeId = 0; nodeId < model->nodes.getSize(); nodeId++)
  {
    // recuperation du noeud courant
    node = model->nodes(nodeId);
#ifdef VERIF_assert
    assert(node != NULL);
#endif

    // prediction de la densite
   // node->newField->density = node->currentField->density + ((1.0 - alpha) * timeStep) * node->currentField->densityInc;

    // prediction de l'energie
    node->newField->energy = node->currentField->energy + ((1.0 - dzeta) * timeStep) * node->currentField->energyInc;

    // application des conditions aux limites imposees
    if (node->boundary != NULL)
      node->boundary->applyConstantOnCurrentFields(node, model->currentTime, timeStep);

    // prediction de la vitesse materielle
    node->newField->speed = node->currentField->speed + ((1.0 - gamma) * timeStep) * node->currentField->acceleration;

    node->newField->displacementInc = 0.0;
    node->newField->displacementInc -= node->currentField->displacement;

    node->newField->displacement = timeStep * node->currentField->speed + (1.0 - beta) * dnlSquare(timeStep) * node->currentField->acceleration + node->currentField->displacement;
    node->currentField->displacement = node->newField->displacement;

    node->newField->displacementInc += node->newField->displacement;
  }
}

//-----------------------------------------------------------------------------
void ExplicitOld::solve(double solveUpToTime)
//-----------------------------------------------------------------------------
{
  assert(model != NULL);

  dynelaData->logFile << "Solve up to " << solveUpToTime << "\n";

   // Get the final time of computation
  _solveUpToTime = solveUpToTime;

 // Compute Jacobian of the elements
  model->computeJacobian();

  // Compute the Mass Matrix if not already computed
  model->computeMassMatrix();

  // Compute the Time Step enforcing computation
  computeTimeStep(true);

  while (model->currentTime < _solveUpToTime)
  {
    // calcul du time step minimal de la structure
    computeTimeStep();

    if ((currentIncrement % _reportFrequency == 0) || (currentIncrement == 1))
    {
      printf("%s inc=%ld time=%8.4E timeStep=%8.4E\n", model->name.chars(), currentIncrement, model->currentTime, timeStep);
    }

    // Compute predictions
    computePredictions();

    // incrementation du nombre d'increments
    currentIncrement++;

    updateGrid();

    computeStrains();

    model->computeJacobian();

    computeConstitutive();

    computeState();

    computeStress();

    computeMass();

    computeMomentum();

    //          computeEnergy ();

    computeCorrections();

    //      computeTemperatures ();

    //      scanInterfaces ();

    //      computeContactForces ();

    updateTime();

    model->writeHistoryFiles();

    model->transfertQuantities();
  }

  /*   // cpu times record
  static CPUrecord recordTimes;

  // first we set the up-time
  upTime=_upTime;

  // premier calcul de determinant
  model->computeJacobian (); OK

  while(timeIsBetweenBounds ())
    {
      // incrementation du nombre d'increments
      increment++; 

      // calcul du time step minimal de la structure
#ifdef computeTimes
      recordTimes.start("Time step");
#endif
      computeTimeStep ();
#ifdef computeTimes
      recordTimes.stop("Time step");
#endif

      // affichage de l'increment courant tous les 100 increments
#ifndef PRINT_Execution_Solve
      if ((increment % frequencyReports == 0) || (increment == 1))
	{
#endif
	  printf ("%s inc=%ld time=%8.4E timeStep=%8.4E\n", model->name.chars(), increment,
		  model->getCurrentTime(), timeStep);

	  // write the progress file
	  progressWrite();
#ifndef PRINT_Execution_Solve
 	}
#endif

      // phase dite de prediction correspondant e l'algorithme explicite
#ifdef computeTimes
      recordTimes.start("Predictions");
#endif
      computePredictions ();
#ifdef computeTimes
      recordTimes.stop("Predictions");
#endif

      // mise a jour des coordonnees de la grille
#ifdef computeTimes
      recordTimes.start("Grid motion");
#endif
      updateGrid ();
#ifdef computeTimes
      recordTimes.stop("Grid motion");
#endif

      // calcul des deformations
#ifdef computeTimes
      recordTimes.start("Strains compute");
#endif
      computeStrains ();
#ifdef computeTimes
      recordTimes.stop("Strains compute");
#endif


      // calcul du modele parent et de getDeterminant J
#ifdef computeTimes
      recordTimes.start("DetJ computation");
#endif
      model->computeJacobian ();
#ifdef computeTimes
      recordTimes.stop("DetJ computation");
#endif

      // integration des lois constitutives
#ifdef computeTimes
      recordTimes.start("Compute constitutive");
#endif
      computeConstitutive ();
#ifdef computeTimes
      recordTimes.stop("Compute constitutive");
#endif

      // integration de la loi d'etat
#ifdef computeTimes
      recordTimes.start("State law");
#endif
      computeState ();
#ifdef computeTimes
      recordTimes.stop("State law");
#endif

      // calcul des contraintes
#ifdef computeTimes
      recordTimes.start("Stress computation");
#endif
      computeStress ();
#ifdef computeTimes
      recordTimes.stop("Stress computation");
#endif


      // integration de la masse
#ifdef computeTimes
      recordTimes.start("Mass integration");
#endif
      computeMass ();
#ifdef computeTimes
      recordTimes.stop("Mass integration");
#endif


      // integration de la quantite de mouvement
#ifdef computeTimes
      recordTimes.start("Momentum integration");
#endif
      computeMomentum ();
#ifdef computeTimes
      recordTimes.stop("Momentum integration");
#endif

      // integration de l'energie et calcul des masses nodales pour le contact
#ifdef computeTimes
      recordTimes.start("Energy integration");
#endif
      computeEnergy ();
#ifdef computeTimes
      recordTimes.stop("Energy integration");
#endif

     // phase de correction
#ifdef computeTimes
      recordTimes.start("Corrections");
#endif
      computeCorrections ();
#ifdef computeTimes
      recordTimes.stop("Corrections");
#endif

      // calcul des temperatures
#ifdef computeTimes
      recordTimes.start("Temperatures");
#endif
      computeTemperatures ();
#ifdef computeTimes
      recordTimes.stop("Temperatures");
#endif

      // scan des interfaces
#ifdef computeTimes
      recordTimes.start("Interfaces");
#endif
      scanInterfaces ();
#ifdef computeTimes
      recordTimes.stop("Interfaces");
#endif

      // calcul des efforts de contact
#ifdef computeTimes
      recordTimes.start("Contact forces");
#endif
      computeContactForces ();
#ifdef computeTimes
      recordTimes.stop("Contact forces");
#endif

      // update du time
      updateTime ();

      // history file
      fprintf (model->history_file, "%8.4E %8.4E %8.4E %8.4E\n", model->getCurrentTime(), 
	       timeStep , model->getTotalMass (), model->getTotalKineticEnergy ());
      fflush (model->history_file);

#ifdef computeTimes
      recordTimes.start("History files");
#endif
      model->writeHistoryFiles();
#ifdef computeTimes
      recordTimes.stop("History files");
#endif

      // transfert des valeurs d'un increment a l'autre
#ifdef computeTimes
      recordTimes.start("Transfert increment");
#endif
      model->transfertQuantities ();
#ifdef computeTimes
      recordTimes.stop("Transfert increment");
#endif

  }

  // print the CPU times
  recordTimes.report("CPU-TIMES");*/
}

//-----------------------------------------------------------------------------
void ExplicitOld::updateGrid()
//-----------------------------------------------------------------------------
{
#ifdef PRINT_Execution_Solve
  cout << "Grid motion computation\n";
#endif

  // mise e jour de la position des noeuds
  for (long nodeId = 0; nodeId < model->nodes.getSize(); nodeId++)
    model->nodes(nodeId)->coordinates += model->nodes(nodeId)->newField->displacementInc;
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeStrains()
//-----------------------------------------------------------------------------
{
#ifdef PRINT_Execution_Solve
  cout << "Strains computation\n";
#endif

  for (long elementId = 0; elementId < model->elements.getSize(); elementId++)
  {
   // model->elements(elementId)->computeStrainsOld(timeStep);
  }
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeConstitutive()
//-----------------------------------------------------------------------------
{
  Element *element;

#ifdef PRINT_Execution_Solve
  cout << "Constitutive equation\n";
#endif

  for (long elementId = 0; elementId < model->elements.getSize(); elementId++)
  {
    // chargement de l'element en cours
    element = model->elements(elementId);

    // integration de la loi constitutive sur l'element courant
    //element->computeConstitutiveEquation();
  }
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeState()
//-----------------------------------------------------------------------------
{
#ifdef PRINT_Execution_Solve
  cout << "State equation\n";
#endif

  for (long elementId = 0; elementId < model->elements.getSize(); elementId++)
  {

    // integration de la loi d'etat sur l'element courant
    //model->elements(elementId)->computeStateEquationOld();
  }
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeStress()
//-----------------------------------------------------------------------------
{
/*   Element *element;
  Material *pmat;
  long elementId, pt;
  IntegrationPoint *fpt;

  Tensor2 devEpsPlas;
  Tensor2 Id;
  Tensor2 en1;
  double f_trial_1;
  Tensor2 n_1;
  Tensor2 ksi_1;
  double Gamma;

  Id.setToUnity();
 */
#ifdef PRINT_Execution_Solve
  cout << "Stresses computation\n";
#endif

  // boucle sur les elements de la structure
  for (long elementId = 0; elementId < model->elements.getSize(); elementId++)
  {

    // pointeur sur l'element courant
    //model->elements(elementId)->computeStressOld(timeStep);
  }
  /*#ifdef VERIF_assert
    assert(element != NULL);
#endif

    // recuperation du materiau de l'element
    pmat = element->material;
#ifdef VERIF_assert
    assert(pmat != NULL);
#endif

    // test de la loi d'ecoulement elastique ou elastoplastique
    if (!pmat->isYield())
    {
    }
    else
    {
      // boucle sur les points d'integration
      Tensor2 fi_e;

      for (pt = 0; pt < element->getNumberOfIntegrationPoints(); pt++)
      {

        // Tensor2 fi_e;

        // recup du pointeur sur le tableau de valeurs
        fpt = element->integrationPoints(pt);

        // Nouvel algorithme developpe en 2000

        // decomposition de la contrainte
        fi_e = fpt->S;

        // critere de plasticite
        f_trial_1 =
            fi_e.getNorm() - sqrt(2. / 3.) * pmat->getYieldStress(fpt);
        if (f_trial_1 <= 0.)
        {
        }
        else
        {
          // normale exterieure
          n_1 = fi_e / fi_e.getNorm();

          // calcul de Gamma cas lineaire
          if (pmat->isLinear())
          {
            Gamma =
                (f_trial_1) / (2. * pmat->mu() *
                               (1. +
                                pmat->getDerYieldStress(fpt) / (3. *
                                                                pmat->mu())));
            //  fpt->alpha+=sqrt(2./3.)*Gamma;
          }

          // calcul de Gamma cas non-lineaire
          else
          {
            double fun = 1;
            double dfun;
            double evp0 = fpt->plasticStrain;
            double xi = fi_e.getNorm();
            Gamma = 0;
            int it = 0;

            while (dnlAbs(fun) > 0.0001)
            {
              //version Simo
              fun =
                  -sqrt(2. / 3.) * pmat->getYieldStress(fpt) +
                  xi - 2. * pmat->mu() * Gamma;
              dfun =
                  +2. * pmat->mu() +
                  (2. / 3.) * pmat->getDerYieldStress(fpt);
              Gamma += fun / dfun;
              if ((it++) > 50)
                fatalError("Too many atempts in plastic procedure",
                           "not converged");
              fpt->plasticStrain = evp0 + sqrt(2. / 3.) * Gamma;
            }
            fpt->plasticStrain = evp0;
          }

          fpt->S -= (2. * pmat->mu() * Gamma) * n_1;
          fpt->plasticStrain += sqrt(2. / 3.) * Gamma;

          // mise a jour de la deformation plastique
          // ca n'a pas l'air tres au point !!!
          fpt->PlasticStrain += Gamma * n_1;
          fpt->PlasticStrainInc = Gamma * n_1 / timeStep;
        }
      }
    }

    // RFI
    for (pt = 0; pt < element->getNumberOfIntegrationPoints(); pt++)
    {

      fpt = element->integrationPoints(pt);
      //version 2
      fpt->S = fpt->R * (fpt->S * fpt->R.getTranspose());
      fpt->Stress = fpt->S + fpt->p * Id;

      // RFI des deformations
      fpt->Strain = fpt->R * (fpt->Strain * fpt->R.getTranspose());
      //version 2
      fpt->PlasticStrain = fpt->R * (fpt->PlasticStrain * fpt->R.getTranspose());
      fpt->PlasticStrainInc = fpt->R * (fpt->PlasticStrainInc * fpt->R.getTranspose());
    }
  }*/
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeMass()
//-----------------------------------------------------------------------------
{
  Element *element;
  Node *node;
  long nonodes;
  long nodeId, elementId, glob;

#ifdef PRINT_Execution_Solve
  cout << "Mass equation\n";
#endif

  // matrice M, et vecteur F
  MatrixDiag M(model->nodes.getSize(), model->nodes.getSize());
  Vector F(model->nodes.getSize());
  Vector roInc(model->nodes.getSize());

  for (elementId = 0; elementId < model->elements.getSize(); elementId++)
  {

    // chargement de l'element en cours
    element = model->elements(elementId);

    // nombre de noeuds de l'element
    nonodes = element->getNumberOfNodes();

    // matrice Me, et vecteur Fe
    MatrixDiag Me(nonodes, nonodes);
    Vector Fe(nonodes);

    // integration de la conservation de la masse
    // sur l'element courant
    //element->computeMassEquation(Me, Fe);

    // assemblage de la matrice de masse
    // et du vecteur
    for (nodeId = 0; nodeId < nonodes; nodeId++)
    {
      // recuperation du numero global
      glob = element->nodes(nodeId)->internalNumber();

      // assemblage de M
      M(glob) += Me(nodeId);

      // assemblage de F
      F(glob) += Fe(nodeId);
    }
  }

  // resolution du systeme M x q = F
  roInc = M.getSolve(F);

  // update du champ de densites
  for (nodeId = 0; nodeId < model->nodes.getSize(); nodeId++)
  {
    node = model->nodes(nodeId);
    //node->newField->densityInc = roInc(nodeId);
  }
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeMomentum()
//-----------------------------------------------------------------------------
{
  Element *element;
  Node *node;
  long nonodes;
  long i, j, el, glob;

#ifdef PRINT_Execution_Solve
  cout << "Momentum equation\n";
#endif

  // matrice M, et vecteur F
  MatrixDiag M(model->getNumberOfDimensions() * model->nodes.getSize(), model->getNumberOfDimensions() * model->nodes.getSize());
  Vector F(model->getNumberOfDimensions() * model->nodes.getSize());
  Vector vInc(model->getNumberOfDimensions() * model->nodes.getSize());

  // phase de calcul
  for (el = 0; el < model->elements.getSize(); el++)
  {

    // chargement de l'element en cours
    element = model->elements(el);

    // nombre de noeuds de l'element
    nonodes = element->getNumberOfNodes();

    // matrice Me, et vecteur Fe
    MatrixDiag Me(model->getNumberOfDimensions() * nonodes, model->getNumberOfDimensions() * nonodes);
    Vector Fe(model->getNumberOfDimensions() * nonodes);

    // integration de la conservation de la quantite de mouvement
    // sur l'element courant
    //element->computeMomentumEquation(Me, Fe);

    // assemblage de la matrice de masse
    // et du vecteur
    for (i = 0; i < nonodes; i++)
    {
      // recuperation du numero global
      glob = (element->nodes(i)->internalNumber()) * model->getNumberOfDimensions();

      for (j = 0; j < model->getNumberOfDimensions(); j++)
      {

        // assemblage de M
        M(glob + j) += Me(i * model->getNumberOfDimensions() + j);

        // assemblage de F
        F(glob + j) += Fe(i * model->getNumberOfDimensions() + j);
      }
    }
  }

  // resolution du systeme M x q = F
  vInc = M.getSolve(F);

  // update du champ des accelerations
  for (i = 0; i < model->nodes.getSize(); i++)
  {
    node = model->nodes(i);

    // mise a jour des accelerations
    for (j = 0; j < model->getNumberOfDimensions(); j++)
      node->newField->acceleration(j) = vInc(i * model->getNumberOfDimensions() + j);
  }
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeCorrections()
//-----------------------------------------------------------------------------
{
  long i;
  Node *pnd;

#ifdef PRINT_Execution_Solve
  cout << "Corrections de density, e, speed et u\n";
#endif

  double alpha = 0.;
  double dzeta = 0.;
  double gamma = 0.;
  double beta = 0.;

  // phase de correction
  for (i = 0; i < model->nodes.getSize(); i++)
  {

    // recuperation du noeud courant
    pnd = model->nodes(i);

    // correction de la densite
    //pnd->newField->density += (alpha * timeStep) * pnd->newField->densityInc;

    // correction de l'energie
    pnd->newField->energy += (dzeta * timeStep) * pnd->newField->energyInc;

    pnd->newField->speed += (gamma * timeStep) * pnd->newField->acceleration;
    pnd->newField->displacement += (beta * dnlSquare(timeStep)) * pnd->newField->acceleration;
  }
}

//-----------------------------------------------------------------------------
void ExplicitOld::updateTime()
//-----------------------------------------------------------------------------
{
  model->currentTime += timeStep;
}

/*
//-----------------------------------------------------------------------------
void ExplicitOld::computeForces()
//-----------------------------------------------------------------------------
{
  
}

//-----------------------------------------------------------------------------
void ExplicitOld::computeState ()
//-----------------------------------------------------------------------------
{
  long el;

#ifdef PRINT_Execution_Solve
  cout << "State equation\n";
#endif

  for (el = 0; el < model->elements.getSize (); el++)
    {

 // integration de la loi d'etat sur l'element courant
 model->elements (el)->computeStateEquationOld ();
    }
}

//!Phase de calcul des contraintes
/*!
Cette methode effectue la phase de calcul des contraintes sur la structure. Deux cas se presentent selon la nature du materiau, elastique ou elastoplastique. Dans le cas elastoplastique, on utilise un algorithme de retour radial.
La methode de la rotation finale instantanee est utilisee pour traiter les problemes d'objectivite de la loi de comportement.




//-----------------------------------------------------------------------------
void ExplicitOld::computeEnergy ()
//-----------------------------------------------------------------------------
{
  Element *pel;
  Node *pnd;
  long nonodes;
  long i, el, glob;

#ifdef PRINT_Execution_Solve
  cout << "Energy equation\n";
#endif

  // matrice M, et vecteur F
  MatrixDiag M (model->nodes.getSize (), model->nodes.getSize ());
  Vector F (model->nodes.getSize ());
  Vector eInc (model->nodes.getSize ());

  for (el = 0; el < model->elements.getSize (); el++)
    {

 // chargement de l'element en cours
 pel = model->elements (el);

 // nombre de noeuds de l'element
 nonodes = pel->getNumberOfNodes();

 // matrice Me, et vecteur Fe
 MatrixDiag Me (nonodes, nonodes);
 Vector Fe (nonodes);

 // integration de la conservation de la masse
 // sur l'element courant
 pel->computeEnergyEquation (Me, Fe);

 // assemblage de la matrice de masse
 // et du vecteur
 for (i = 0; i < nonodes; i++)
	{

	  // recuperation du numero global
	  glob = pel->nodes (i)->Id;

	  // assemblage de M
	  M (glob) += Me (i);

	  // assemblage de F
	  F (glob) += Fe (i);
	}
    }

  // resolution du systeme M x q = F
  eInc = M.Solve (F);
  //    cout << eInc <<endl;

  // update du champ des energies
  for (i = 0; i < model->nodes.getSize (); i++)
    {
 pnd = model->nodes (i);
 pnd->newField->energyInc = eInc (i);

 // mise a jour de la masse nodale
 pnd->mass = M (i);
    }
}






//-----------------------------------------------------------------------------
void ExplicitOld::computeTemperatures ()
//-----------------------------------------------------------------------------
{
  Node *pnd;
  Material *pmat;
  long no;
  double coeff;

  coeff = 1.0;

  for (no = 0; no < model->nodes.getSize (); no++)
    {

 // recuperation du noeud
 pnd = model->nodes (no);

 // chargement du materiau
 pmat = pnd->elements (0)->material;

 // calcul de la temperature
 pnd->newField->T = pnd->initialTemperature + coeff * pnd->newField->energy / pmat->heatCoeff ();
    }
}
//-----------------------------------------------------------------------------
void ExplicitOld::scanInterfaces ()
//-----------------------------------------------------------------------------
{
#ifdef PRINT_Execution_Solve
  cout << "Scanning des interfaces\n";
#endif

  // recherche des penetrations d'elements
  // sur la liste des interfaces du modele
  for (long it = 0; it < model->interfaces.getSize (); it++)
    {
 model->interfaces (it)->ScanIncludeNodes ();
    }

  // calculs

}
//-----------------------------------------------------------------------------
void ExplicitOld::computeContactForces ()
//-----------------------------------------------------------------------------
{
  long no;

#ifdef PRINT_Execution_Solve
  cout << "contact forces computation\n";
#endif

  for (no = 0; no < model->nodes.getSize (); no++)
    {
 model->nodes (no)->motion->computeForces (timeStep);
    }
}
*/