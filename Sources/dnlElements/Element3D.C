/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Element3D.C
  \brief Definition file for the Element3D class

  This file is the definition file for the Element3D class.

  \ingroup dnlElements
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <Element3D.h>
#include <Node.h>
#include <NodalField.h>

//-----------------------------------------------------------------------------
Element3D::Element3D(long elementNumber) : Element(elementNumber)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Element3D::Element3D(const Element3D &element) : Element(element)
//-----------------------------------------------------------------------------
{
  std::cout << "recopie d'un Element3D\n";
}

//-----------------------------------------------------------------------------
Element3D::~Element3D()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
double Element3D::getRadiusAtIntegrationPoint()
//-----------------------------------------------------------------------------
{
  fatalError("Element3D::getRadiusAtIntegrationPoint",
             "Method is not callable\n");
  return 0.0;
}

//-----------------------------------------------------------------------------
bool Element3D::computeJacobian()
//-----------------------------------------------------------------------------
{
  Node *node;
const IntegrationPointData* integrationPointData;

  for (short intPtId = 0; intPtId < integrationPoints.getSize(); intPtId++)
  {
    // recuperation du point d'integration courant
    setCurrentIntegrationPoint(intPtId);
integrationPointData = &_elementData->integrationPoint[intPtId];

    // initialisation de J
    _integrationPoint->JxW = 0.0;

    for (short nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
    {
      node = nodes(nodeId);
      _integrationPoint->JxW(0, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(0);
      _integrationPoint->JxW(0, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(0);
      _integrationPoint->JxW(0, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(0);
      _integrationPoint->JxW(1, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(1);
      _integrationPoint->JxW(1, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(1);
      _integrationPoint->JxW(1, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(1);
      _integrationPoint->JxW(2, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(2);
      _integrationPoint->JxW(2, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(2);
      _integrationPoint->JxW(2, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(2);
    }

    // determinant de J
    _integrationPoint->detJ = _integrationPoint->JxW.getDeterminant3x3();

    // test de positivite du Jacobien
    if (_integrationPoint->detJ < 0.0)
    {
      std::cerr << "Negative value of detJ encountered in element " << number << " at integration point " << intPtId + 1 << std::endl;
      return false;
    }

    // calcul de l'inverse de J
    _integrationPoint->JxW.computeInverse3x3(_integrationPoint->detJ, _integrationPoint->invJxW);

    // recalcul de dShapeFunction
    _integrationPoint->dShapeFunction = integrationPointData->derShapeFunction * _integrationPoint->invJxW;
  }
  return true;
}

//-----------------------------------------------------------------------------
void Element3D::computeDeformationGradient(Tensor2 &F, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long nodeId;

  // initialisation a zero de F
  F.setToUnity();

  // calcul de F
  for (nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    F(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacementInc(0);
    F(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacementInc(0);
    F(0, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacementInc(0);
    F(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacementInc(1);
    F(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacementInc(1);
    F(1, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacementInc(1);
    F(2, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacementInc(2);
    F(2, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacementInc(2);
    F(2, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacementInc(2);
  }
}

//-----------------------------------------------------------------------------
bool Element3D::checkLevel2()
//-----------------------------------------------------------------------------
{
  return true;
}

//-----------------------------------------------------------------------------
void Element3D::getV_atIntPoint(Vec3D &v, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;

  // initialisation a zero de v
  v = 0.0;

  // calcul de v
  for (short nodeId = 0; nodeId < nodes.getSize(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    v(0) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(0));
    v(1) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(1));
    v(2) += _integrationPoint->integrationPointData->shapeFunction(nodeId) * (field->speed(2));
  }
}

//-----------------------------------------------------------------------------
void Element3D::getdV_atIntPoint(Tensor2 &dv, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;

  // initialisation a zero de dv
  dv = 0.0;

  // calcul de dv
  for (short nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
  {
    field = nodes(nodeId)->getNodalField(time);
    dv(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(0);
    dv(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(0);
    dv(0, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->speed(0);
    dv(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(1);
    dv(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(1);
    dv(1, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->speed(1);
    dv(2, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->speed(2);
    dv(2, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->speed(2);
    dv(2, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->speed(2);
  }
}

/*




//-----------------------------------------------------------------------------
void Element3D::getU_atIntPoint (Vec3D & u, short time)
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
       u (2) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacementInc (2);
    }
}




//-----------------------------------------------------------------------------
void Element3D::getdU_atIntPoint (Tensor2 & du, short time)
//-----------------------------------------------------------------------------
{
  NodalField *field;
  long k;

  // initialisation a zero de du
  du = 0.0;

  // calcul de du
  for (k = 0; k < getNumberOfNodes(); k++)
    {
      field = nodes (k)->getNodalField (time);
      du (0, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacementInc (0);
      du (0, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacementInc (0);
      du (0, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacementInc (0);
      du (1, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacementInc (1);
      du (1, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacementInc (1);
      du (1, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacementInc (1);
      du (2, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacementInc (2);
      du (2, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacementInc (2);
      du (2, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacementInc (2);
    }
}


//-----------------------------------------------------------------------------
void Element3D::computeElasticStiffnessMatrix (Matrix & K, bool under)
//-----------------------------------------------------------------------------
{
  long pt;
  double WxdJ;
  long i,I,j,J;
  long pts;
  IntegrationPointBase* pInt;

  // initialisation
  K.redim(getNumberOfDimensions()*getNumberOfNodes(),getNumberOfDimensions()*getNumberOfNodes());
  K=0;
  Matrix C;

  C=material->getHookeMatrix(Material::threedimensional);

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

      // calcul de C.B
      for (i = 0; i < getNumberOfNodes(); i++)
	{
	  I=getNumberOfDimensions()*i;
	  CB(0,I) = (C(0,0)*pInt->dShapeFunction(i,0)+C(0,3)*pInt->dShapeFunction(i,1)+C(0,5)*pInt->dShapeFunction(i,2));
	  CB(1,I) = (C(1,0)*pInt->dShapeFunction(i,0)+C(1,3)*pInt->dShapeFunction(i,1)+C(1,5)*pInt->dShapeFunction(i,2));
	  CB(2,I) = (C(2,0)*pInt->dShapeFunction(i,0)+C(2,3)*pInt->dShapeFunction(i,1)+C(2,5)*pInt->dShapeFunction(i,2));
	  CB(3,I) = (C(3,0)*pInt->dShapeFunction(i,0)+C(3,3)*pInt->dShapeFunction(i,1)+C(3,5)*pInt->dShapeFunction(i,2));
	  CB(4,I) = (C(4,0)*pInt->dShapeFunction(i,0)+C(4,3)*pInt->dShapeFunction(i,1)+C(4,5)*pInt->dShapeFunction(i,2));
	  CB(5,I) = (C(5,0)*pInt->dShapeFunction(i,0)+C(5,3)*pInt->dShapeFunction(i,1)+C(5,5)*pInt->dShapeFunction(i,2));
	  CB(0,I+1) = (C(0,1)*pInt->dShapeFunction(i,1)+C(0,3)*pInt->dShapeFunction(i,0)+C(0,4)*pInt->dShapeFunction(i,2));
	  CB(1,I+1) = (C(1,1)*pInt->dShapeFunction(i,1)+C(1,3)*pInt->dShapeFunction(i,0)+C(1,4)*pInt->dShapeFunction(i,2));
	  CB(2,I+1) = (C(2,1)*pInt->dShapeFunction(i,1)+C(2,3)*pInt->dShapeFunction(i,0)+C(2,4)*pInt->dShapeFunction(i,2));
	  CB(3,I+1) = (C(3,1)*pInt->dShapeFunction(i,1)+C(3,3)*pInt->dShapeFunction(i,0)+C(3,4)*pInt->dShapeFunction(i,2));
	  CB(4,I+1) = (C(4,1)*pInt->dShapeFunction(i,1)+C(4,3)*pInt->dShapeFunction(i,0)+C(4,4)*pInt->dShapeFunction(i,2));
	  CB(5,I+1) = (C(5,1)*pInt->dShapeFunction(i,1)+C(5,3)*pInt->dShapeFunction(i,0)+C(5,4)*pInt->dShapeFunction(i,2));
	  CB(0,I+2) = (C(0,2)*pInt->dShapeFunction(i,2)+C(0,4)*pInt->dShapeFunction(i,1)+C(0,5)*pInt->dShapeFunction(i,0));
	  CB(1,I+2) = (C(1,2)*pInt->dShapeFunction(i,2)+C(1,4)*pInt->dShapeFunction(i,1)+C(1,5)*pInt->dShapeFunction(i,0));
	  CB(2,I+2) = (C(2,2)*pInt->dShapeFunction(i,2)+C(2,4)*pInt->dShapeFunction(i,1)+C(2,5)*pInt->dShapeFunction(i,0));
	  CB(3,I+2) = (C(3,2)*pInt->dShapeFunction(i,2)+C(3,4)*pInt->dShapeFunction(i,1)+C(3,5)*pInt->dShapeFunction(i,0));
	  CB(4,I+2) = (C(4,2)*pInt->dShapeFunction(i,2)+C(4,4)*pInt->dShapeFunction(i,1)+C(4,5)*pInt->dShapeFunction(i,0));
	  CB(5,I+2) = (C(5,2)*pInt->dShapeFunction(i,2)+C(5,4)*pInt->dShapeFunction(i,1)+C(5,5)*pInt->dShapeFunction(i,0));
	}

      // calcul de BT [C B]
      for (i = 0; i < getNumberOfNodes(); i++)
	for (j = 0; j < getNumberOfNodes(); j++)
	  {
	    I=getNumberOfDimensions()*i;
	    J=getNumberOfDimensions()*j;
	    K(I,J) += (pInt->dShapeFunction(i,0) * CB (0,J) + pInt->dShapeFunction(i,1) * CB (3,J) + pInt->dShapeFunction(i,2) * CB (5,J))*WxdJ;
	    K(I,J+1) += (pInt->dShapeFunction(i,0) * CB (0,J+1) + pInt->dShapeFunction(i,1) * CB (3,J+1) + pInt->dShapeFunction(i,2) * CB (5,J+1))*WxdJ;
	    K(I,J+2) += (pInt->dShapeFunction(i,0) * CB (0,J+2) + pInt->dShapeFunction(i,1) * CB (3,J+2) + pInt->dShapeFunction(i,2) * CB (5,J+2))*WxdJ;
	    K(I+1,J) += (pInt->dShapeFunction(i,1) * CB (1,J) + pInt->dShapeFunction(i,0) * CB (3,J) + pInt->dShapeFunction(i,2) * CB (4,J))*WxdJ;
	    K(I+1,J+1) += (pInt->dShapeFunction(i,1) * CB (1,J+1) + pInt->dShapeFunction(i,0) * CB (3,J+1) + pInt->dShapeFunction(i,2) * CB (4,J+1))*WxdJ;
	    K(I+1,J+2) += (pInt->dShapeFunction(i,1) * CB (1,J+2) + pInt->dShapeFunction(i,0) * CB (3,J+2) + pInt->dShapeFunction(i,2) * CB (4,J+2))*WxdJ;
	    K(I+2,J) += (pInt->dShapeFunction(i,2) * CB (2,J) + pInt->dShapeFunction(i,1) * CB (4,J) + pInt->dShapeFunction(i,0) * CB (5,J))*WxdJ;
	    K(I+2,J+1) += (pInt->dShapeFunction(i,2) * CB (2,J+1) + pInt->dShapeFunction(i,1) * CB (4,J+1) + pInt->dShapeFunction(i,0) * CB (5,J+1))*WxdJ;
	    K(I+2,J+2) += (pInt->dShapeFunction(i,2) * CB (2,J+2) + pInt->dShapeFunction(i,1) * CB (4,J+2) + pInt->dShapeFunction(i,0) * CB (5,J+2))*WxdJ;
	  }
    }
}
*/