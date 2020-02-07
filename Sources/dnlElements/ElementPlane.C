/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file ElementPlane.C
  \brief Definition file for the ElementPlane class

  This file is the definition file for the ElementPlane class.

  \ingroup dnlElements
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <ElementPlane.h>
#include <Node.h>
#include <NodalField.h>

//-----------------------------------------------------------------------------
ElementPlane::ElementPlane(long elementNumber) : Element(elementNumber)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
ElementPlane::ElementPlane(const ElementPlane &element) : Element(element)
//-----------------------------------------------------------------------------
{
  std::cout << "recopie d'un ElementPlane\n";
}

//-----------------------------------------------------------------------------
ElementPlane::~ElementPlane()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
bool ElementPlane::computeJacobian()
//-----------------------------------------------------------------------------
{
  Node *node;
  const IntegrationPointData *integrationPointData;

  for (short intPtId = 0; intPtId < integrationPoints.getSize(); intPtId++)
  {
    // recuperation du point d'integration courant
    setCurrentIntegrationPoint(intPtId);
    integrationPointData = &_elementData->integrationPoint[intPtId];

    // initialisation de J
    _integrationPoint->JxW = 0.0;

    // calcul de J
    for (short nodeId = 0; nodeId < nodes.getSize(); nodeId++)
    {
      node = nodes(nodeId);
      _integrationPoint->JxW(0, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(0);
      _integrationPoint->JxW(0, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(0);
      _integrationPoint->JxW(1, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(1);
      _integrationPoint->JxW(1, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(1);
    }

    // determinant de J
    _integrationPoint->detJ = _integrationPoint->JxW.getDeterminant2x2();

    // test de positivite du Jacobien
    if (_integrationPoint->detJ < 0.0)
    {
      std::cerr << "Negative value of detJ encountered in element " << number << " at integration point " << intPtId + 1 << std::endl;
      return false;
    }

    // calcul de l'inverse de J
    _integrationPoint->JxW.computeInverse2x2(_integrationPoint->detJ, _integrationPoint->invJxW);

    // recalcul de dShapeFunction
    _integrationPoint->dShapeFunction = integrationPointData->derShapeFunction * _integrationPoint->invJxW;
  }
  return true;
}

//-----------------------------------------------------------------------------
void ElementPlane::getV_atIntPoint(Vec3D &v, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;

  // initialisation a zero de v
  v = 0.0;

  // calcul de v courant
  for (short nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    v(0) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(0));
    v(1) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(1));
  }
}

/*

//-----------------------------------------------------------------------------
void ElementPlane::computeElasticStiffnessMatrix (Matrix & K, bool under)
//-----------------------------------------------------------------------------
{
  long pt;
  double WxdJ, R;
  long i,I,j,J;
  long pts;
  IntegrationPointBase* pInt;

  // initialisation
  K.redim(getNumberOfDimensions()*getNumberOfNodes(),getNumberOfDimensions()*getNumberOfNodes());
  K=0;
  Matrix C;

  // chargement de la matrice de comportement C
  if (getFamily() == Element::Axisymetric)
    C=material->getHookeMatrix(Material::axisymetric);
  else 
    C=material->getHookeMatrix(Material::planeStrain);

  // matrice temporaire
  Matrix CB(C.rows(),getNumberOfDimensions()*getNumberOfNodes());

  // nomre de points d'integration
  if (under)
    {
      pts=numberOfUnderIntegrationPoints ();
    }
  else
    {
      pts=integrationPoints.getSize ();
    }

  // parallelisation
  //#pragma omp parallel for private(WxdJ),shared(K)
  // boucle sur les points d'integration
  for (pt = 0; pt < pts; pt++)
    {

      // recuperation du point d'integration
      if (under)
	{
	  getUnderIntegrationPoint (pt);
	  pInt=underIntegrationPoint;
	}
      else 
	{
	  setCurrentIntegrationPoint (pt);
	  pInt=ref;
	}

      // calcul du terme d'integration numerique
      WxdJ = pInt->integrationPointData->weight * pInt->detJ;
      if (getFamily() == Element::Axisymetric)
	{
	  R=getRadiusAtIntegrationPoint();
	  WxdJ *= 2 * PI * R;
	}

      // calcul de C.B
      for (i = 0; i < getNumberOfNodes(); i++)
	{
	  I=getNumberOfDimensions()*i;
	  CB(0,I) = (C(0,0)*pInt->dShapeFunction(i,0)+C(0,2)*pInt->dShapeFunction(i,1));
	  CB(1,I) = (C(1,0)*pInt->dShapeFunction(i,0)+C(1,2)*pInt->dShapeFunction(i,1));
	  CB(2,I) = (C(2,0)*pInt->dShapeFunction(i,0)+C(2,2)*pInt->dShapeFunction(i,1));
	  CB(0,I+1) = (C(0,1)*pInt->dShapeFunction(i,1)+C(0,2)*pInt->dShapeFunction(i,0));
	  CB(1,I+1) = (C(1,1)*pInt->dShapeFunction(i,1)+C(1,2)*pInt->dShapeFunction(i,0));
	  CB(2,I+1) = (C(2,1)*pInt->dShapeFunction(i,1)+C(2,2)*pInt->dShapeFunction(i,0));
	  if (getFamily() == Element::Axisymetric)
	    {
	      CB(3,I) = (C(3,0)*pInt->dShapeFunction(i,0)+C(3,2)*pInt->dShapeFunction(i,1));
	      CB(3,I+1) = (C(3,1)*pInt->dShapeFunction(i,1)+C(3,2)*pInt->dShapeFunction(i,0));
	      CB(0,I) += C(0,3)*pInt->integrationPointData->shapeFunction(i)/R;
	      CB(1,I) += C(1,3)*pInt->integrationPointData->shapeFunction(i)/R;
	      CB(2,I) += C(2,3)*pInt->integrationPointData->shapeFunction(i)/R;
	      CB(3,I) += C(3,3)*pInt->integrationPointData->shapeFunction(i)/R;
	    }
	}

      // calcul de BT [C B]
      for (i = 0; i < getNumberOfNodes(); i++)
	for (j = 0; j < getNumberOfNodes(); j++)
	  {
	    I=getNumberOfDimensions()*i;
	    J=getNumberOfDimensions()*j;
	    K(I,J) += (pInt->dShapeFunction(i,0) * CB (0,J) + pInt->dShapeFunction(i,1) * CB (2,J))*WxdJ;
	    K(I,J+1) += (pInt->dShapeFunction(i,0) * CB (0,J+1) + pInt->dShapeFunction(i,1) * CB (2,J+1))*WxdJ;
	    K(I+1,J) += (pInt->dShapeFunction(i,1) * CB (1,J) + pInt->dShapeFunction(i,0) * CB (2,J))*WxdJ;
	    K(I+1,J+1) += (pInt->dShapeFunction(i,1) * CB (1,J+1) + pInt->dShapeFunction(i,0) * CB (2,J+1))*WxdJ;
	    if (getFamily() == Element::Axisymetric)
	      {
		K(I,J) += CB(3,J)*pInt->integrationPointData->shapeFunction(i)/R*WxdJ;
		K(I,J+1) += CB(3,J+1)*pInt->integrationPointData->shapeFunction(i)/R*WxdJ;
	      }
	  }
    }
}


//-----------------------------------------------------------------------------
void ElementPlane::getU_atIntPoint (Vec3D & u, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long i;

  // initialisation a zero de du
  u = 0.0;

  // calcul de du
  for (i = 0; i < nodes.getSize (); i++)
    {
      field = nodes (i)->getNodalField (time);
       u (0) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacementInc (0);
       u (1) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacementInc (1);
    }
}
*/