/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
// TODOCXYFILE

/*!
  \file Element3D.C
  \brief Definition file for the Element3D class

  This file is the definition file for the Element3D class.

  \ingroup dnlElements
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
double Element3D::getRadiusAtUnderIntegrationPoint()
//-----------------------------------------------------------------------------
{
  fatalError("Element3D::getRadiusAtUnderIntegrationPoint",
             "Method is not callable\n");
  return 0.0;
}

//-----------------------------------------------------------------------------
bool Element3D::computeJacobian(bool reference)
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

    if (reference)
      _integrationPoint->detJ0 = _integrationPoint->detJ;
  }
  return true;
}

//-----------------------------------------------------------------------------
bool Element3D::computeUnderJacobian(bool reference)
//-----------------------------------------------------------------------------
{
  Node *node;
  const IntegrationPointData *integrationPointData;

  for (short intPtId = 0; intPtId < underIntegrationPoints.getSize(); intPtId++)
  {
    // recuperation du point d'integration courant
    setCurrentUnderIntegrationPoint(intPtId);
    integrationPointData = &_elementData->underIntegrationPoint[intPtId];

    // initialisation de J
    _underIntegrationPoint->JxW = 0.0;

    for (short nodeId = 0; nodeId < getNumberOfNodes(); nodeId++)
    {
      node = nodes(nodeId);
      _underIntegrationPoint->JxW(0, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(0);
      _underIntegrationPoint->JxW(0, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(0);
      _underIntegrationPoint->JxW(0, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(0);
      _underIntegrationPoint->JxW(1, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(1);
      _underIntegrationPoint->JxW(1, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(1);
      _underIntegrationPoint->JxW(1, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(1);
      _underIntegrationPoint->JxW(2, 0) += integrationPointData->derShapeFunction(nodeId, 0) * node->coordinates(2);
      _underIntegrationPoint->JxW(2, 1) += integrationPointData->derShapeFunction(nodeId, 1) * node->coordinates(2);
      _underIntegrationPoint->JxW(2, 2) += integrationPointData->derShapeFunction(nodeId, 2) * node->coordinates(2);
    }

    // determinant de J
    _underIntegrationPoint->detJ = _underIntegrationPoint->JxW.getDeterminant3x3();

    // test de positivite du Jacobien
    if (_underIntegrationPoint->detJ < 0.0)
    {
      std::cerr << "Negative value of detJ encountered in element " << number << " at integration point " << intPtId + 1 << std::endl;
      return false;
    }

    // calcul de l'inverse de J
    _underIntegrationPoint->JxW.computeInverse3x3(_underIntegrationPoint->detJ, _underIntegrationPoint->invJxW);

    // recalcul de dShapeFunction
    _underIntegrationPoint->dShapeFunction = integrationPointData->derShapeFunction * _underIntegrationPoint->invJxW;

    if (reference)
      _underIntegrationPoint->detJ0 = _underIntegrationPoint->detJ;
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
    F(0, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(0);
    F(0, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(0);
    F(0, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacement(0);
    F(1, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(1);
    F(1, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(1);
    F(1, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacement(1);
    F(2, 0) += _integrationPoint->dShapeFunction(nodeId, 0) * field->displacement(2);
    F(2, 1) += _integrationPoint->dShapeFunction(nodeId, 1) * field->displacement(2);
    F(2, 2) += _integrationPoint->dShapeFunction(nodeId, 2) * field->displacement(2);
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
       u (0) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacement (0);
       u (1) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacement (1);
       u (2) += _integrationPoint->integrationPointData->shapeFunction (i) * field->displacement (2);
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
      du (0, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (0);
      du (0, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (0);
      du (0, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacement (0);
      du (1, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (1);
      du (1, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (1);
      du (1, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacement (1);
      du (2, 0) += _integrationPoint->dShapeFunction (k, 0) * field->displacement (2);
      du (2, 1) += _integrationPoint->dShapeFunction (k, 1) * field->displacement (2);
      du (2, 2) += _integrationPoint->dShapeFunction (k, 2) * field->displacement (2);
    }
}
*/
//-----------------------------------------------------------------------------
void Element3D::computeElasticStiffnessMatrix(bool underIntegration)
//-----------------------------------------------------------------------------
{
  long pt;
  double WxdJ;
  long i, I, j, J;
  long pts;
  IntegrationPointBase *currentIntPoint;

  // initialisation
  stiffnessMatrix = 0;
  Matrix C = material->getHookeMatrix(Material::threedimensional);

  // matrice temporaire
  Matrix CB(C.rows(), getNumberOfDimensions() * getNumberOfNodes());

  // nomre de points d'integration
  if (underIntegration)
  {
    computeUnderJacobian();
    pts = underIntegrationPoints.getSize();
  }
  else
  {
    pts = integrationPoints.getSize();
  }

  // parallelisation
  //#pragma omp parallel for private(WxdJ),shared(stiffnessMatrix)
  // boucle sur les points d'integration
  for (pt = 0; pt < pts; pt++)
  {
    // recuperation du point d'integration
    if (underIntegration)
    {
      currentIntPoint = getUnderIntegrationPoint(pt);
    }
    else
    {
      currentIntPoint = getIntegrationPoint(pt);
    }

    // calcul du terme d'integration numerique
    WxdJ = currentIntPoint->integrationPointData->weight * currentIntPoint->detJ;

    // calcul de C.B
    for (i = 0; i < getNumberOfNodes(); i++)
    {
      I = getNumberOfDimensions() * i;
      CB(0, I) = C(0, 0) * currentIntPoint->dShapeFunction(i, 0);
      CB(3, I) = C(3, 3) * currentIntPoint->dShapeFunction(i, 1);
      CB(5, I) = C(5, 5) * currentIntPoint->dShapeFunction(i, 2);
      CB(0, I + 1) = C(0, 1) * currentIntPoint->dShapeFunction(i, 1);
      CB(1, I + 1) = C(1, 1) * currentIntPoint->dShapeFunction(i, 1);
      CB(2, I + 1) = C(2, 1) * currentIntPoint->dShapeFunction(i, 1);
      CB(3, I + 1) = C(3, 3) * currentIntPoint->dShapeFunction(i, 0);
      CB(4, I + 1) = C(4, 4) * currentIntPoint->dShapeFunction(i, 2);
      CB(0, I + 2) = C(0, 2) * currentIntPoint->dShapeFunction(i, 2);
      CB(1, I + 2) = C(1, 2) * currentIntPoint->dShapeFunction(i, 2);
      CB(2, I + 2) = C(2, 2) * currentIntPoint->dShapeFunction(i, 2);
      CB(4, I + 2) = C(4, 4) * currentIntPoint->dShapeFunction(i, 1);
      CB(5, I + 2) = C(5, 5) * currentIntPoint->dShapeFunction(i, 0);
      /*       CB(0, I) = (C(0, 0) * currentIntPoint->dShapeFunction(i, 0) + C(0, 3) * currentIntPoint->dShapeFunction(i, 1) + C(0, 5) * currentIntPoint->dShapeFunction(i, 2));
      CB(1, I) = (C(1, 0) * currentIntPoint->dShapeFunction(i, 0) + C(1, 3) * currentIntPoint->dShapeFunction(i, 1) + C(1, 5) * currentIntPoint->dShapeFunction(i, 2));
      CB(2, I) = (C(2, 0) * currentIntPoint->dShapeFunction(i, 0) + C(2, 3) * currentIntPoint->dShapeFunction(i, 1) + C(2, 5) * currentIntPoint->dShapeFunction(i, 2));
      CB(3, I) = (C(3, 0) * currentIntPoint->dShapeFunction(i, 0) + C(3, 3) * currentIntPoint->dShapeFunction(i, 1) + C(3, 5) * currentIntPoint->dShapeFunction(i, 2));
      CB(4, I) = (C(4, 0) * currentIntPoint->dShapeFunction(i, 0) + C(4, 3) * currentIntPoint->dShapeFunction(i, 1) + C(4, 5) * currentIntPoint->dShapeFunction(i, 2));
      CB(5, I) = (C(5, 0) * currentIntPoint->dShapeFunction(i, 0) + C(5, 3) * currentIntPoint->dShapeFunction(i, 1) + C(5, 5) * currentIntPoint->dShapeFunction(i, 2));
      CB(0, I + 1) = (C(0, 1) * currentIntPoint->dShapeFunction(i, 1) + C(0, 3) * currentIntPoint->dShapeFunction(i, 0) + C(0, 4) * currentIntPoint->dShapeFunction(i, 2));
      CB(1, I + 1) = (C(1, 1) * currentIntPoint->dShapeFunction(i, 1) + C(1, 3) * currentIntPoint->dShapeFunction(i, 0) + C(1, 4) * currentIntPoint->dShapeFunction(i, 2));
      CB(2, I + 1) = (C(2, 1) * currentIntPoint->dShapeFunction(i, 1) + C(2, 3) * currentIntPoint->dShapeFunction(i, 0) + C(2, 4) * currentIntPoint->dShapeFunction(i, 2));
      CB(3, I + 1) = (C(3, 1) * currentIntPoint->dShapeFunction(i, 1) + C(3, 3) * currentIntPoint->dShapeFunction(i, 0) + C(3, 4) * currentIntPoint->dShapeFunction(i, 2));
      CB(4, I + 1) = (C(4, 1) * currentIntPoint->dShapeFunction(i, 1) + C(4, 3) * currentIntPoint->dShapeFunction(i, 0) + C(4, 4) * currentIntPoint->dShapeFunction(i, 2));
      CB(5, I + 1) = (C(5, 1) * currentIntPoint->dShapeFunction(i, 1) + C(5, 3) * currentIntPoint->dShapeFunction(i, 0) + C(5, 4) * currentIntPoint->dShapeFunction(i, 2));
      CB(0, I + 2) = (C(0, 2) * currentIntPoint->dShapeFunction(i, 2) + C(0, 4) * currentIntPoint->dShapeFunction(i, 1) + C(0, 5) * currentIntPoint->dShapeFunction(i, 0));
      CB(1, I + 2) = (C(1, 2) * currentIntPoint->dShapeFunction(i, 2) + C(1, 4) * currentIntPoint->dShapeFunction(i, 1) + C(1, 5) * currentIntPoint->dShapeFunction(i, 0));
      CB(2, I + 2) = (C(2, 2) * currentIntPoint->dShapeFunction(i, 2) + C(2, 4) * currentIntPoint->dShapeFunction(i, 1) + C(2, 5) * currentIntPoint->dShapeFunction(i, 0));
      CB(3, I + 2) = (C(3, 2) * currentIntPoint->dShapeFunction(i, 2) + C(3, 4) * currentIntPoint->dShapeFunction(i, 1) + C(3, 5) * currentIntPoint->dShapeFunction(i, 0));
      CB(4, I + 2) = (C(4, 2) * currentIntPoint->dShapeFunction(i, 2) + C(4, 4) * currentIntPoint->dShapeFunction(i, 1) + C(4, 5) * currentIntPoint->dShapeFunction(i, 0));
      CB(5, I + 2) = (C(5, 2) * currentIntPoint->dShapeFunction(i, 2) + C(5, 4) * currentIntPoint->dShapeFunction(i, 1) + C(5, 5) * currentIntPoint->dShapeFunction(i, 0)); */
    }

    // calcul de BT [C B]
    for (i = 0; i < getNumberOfNodes(); i++)
      for (j = 0; j < getNumberOfNodes(); j++)
      {
        I = getNumberOfDimensions() * i;
        J = getNumberOfDimensions() * j;
        stiffnessMatrix(I, J) += (currentIntPoint->dShapeFunction(i, 0) * CB(0, J) + currentIntPoint->dShapeFunction(i, 1) * CB(3, J) + currentIntPoint->dShapeFunction(i, 2) * CB(5, J)) * WxdJ;
        stiffnessMatrix(I, J + 1) += (currentIntPoint->dShapeFunction(i, 0) * CB(0, J + 1) + currentIntPoint->dShapeFunction(i, 1) * CB(3, J + 1) + currentIntPoint->dShapeFunction(i, 2) * CB(5, J + 1)) * WxdJ;
        stiffnessMatrix(I, J + 2) += (currentIntPoint->dShapeFunction(i, 0) * CB(0, J + 2) + currentIntPoint->dShapeFunction(i, 1) * CB(3, J + 2) + currentIntPoint->dShapeFunction(i, 2) * CB(5, J + 2)) * WxdJ;
        stiffnessMatrix(I + 1, J) += (currentIntPoint->dShapeFunction(i, 1) * CB(1, J) + currentIntPoint->dShapeFunction(i, 0) * CB(3, J) + currentIntPoint->dShapeFunction(i, 2) * CB(4, J)) * WxdJ;
        stiffnessMatrix(I + 1, J + 1) += (currentIntPoint->dShapeFunction(i, 1) * CB(1, J + 1) + currentIntPoint->dShapeFunction(i, 0) * CB(3, J + 1) + currentIntPoint->dShapeFunction(i, 2) * CB(4, J + 1)) * WxdJ;
        stiffnessMatrix(I + 1, J + 2) += (currentIntPoint->dShapeFunction(i, 1) * CB(1, J + 2) + currentIntPoint->dShapeFunction(i, 0) * CB(3, J + 2) + currentIntPoint->dShapeFunction(i, 2) * CB(4, J + 2)) * WxdJ;
        stiffnessMatrix(I + 2, J) += (currentIntPoint->dShapeFunction(i, 2) * CB(2, J) + currentIntPoint->dShapeFunction(i, 1) * CB(4, J) + currentIntPoint->dShapeFunction(i, 0) * CB(5, J)) * WxdJ;
        stiffnessMatrix(I + 2, J + 1) += (currentIntPoint->dShapeFunction(i, 2) * CB(2, J + 1) + currentIntPoint->dShapeFunction(i, 1) * CB(4, J + 1) + currentIntPoint->dShapeFunction(i, 0) * CB(5, J + 1)) * WxdJ;
        stiffnessMatrix(I + 2, J + 2) += (currentIntPoint->dShapeFunction(i, 2) * CB(2, J + 2) + currentIntPoint->dShapeFunction(i, 1) * CB(4, J + 2) + currentIntPoint->dShapeFunction(i, 0) * CB(5, J + 2)) * WxdJ;
      }
  }
}
