/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Polygon.h
  \brief Declaration file for the Polygon class

  This file is the declaration file for the Polygon class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <Polygon.h>
/* #include <List.h>
#include <DynELA.h>
#include <Node.h>
#include <Element.h>
#include <Field.h>
#include <Model.h>
 */
//-----------------------------------------------------------------------------
Polygon::Polygon()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Polygon::~Polygon()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Polygon::init()
//-----------------------------------------------------------------------------
{
  points = 0;
}

//-----------------------------------------------------------------------------
int Polygon::numberOfPoints()
//-----------------------------------------------------------------------------
{
  return points;
}

//-----------------------------------------------------------------------------
void Polygon::add(Vec3D point)
//-----------------------------------------------------------------------------
{
  if (points >= 8)
  {
    fatalError("No left space in polygon");
  }
  vertices[points++] = point;
}

//-----------------------------------------------------------------------------
void Polygon::add(Node *node)
//-----------------------------------------------------------------------------
{
  if (points >= 8)
  {
    fatalError("No left space in polygon");
  }
  nodes[points] = node;
  vertices[points] = node->coordinates;
  points++;
}

//-----------------------------------------------------------------------------
Vec3D Polygon::getVertex(int vertex)
//-----------------------------------------------------------------------------
{
  return vertices[vertex];
}

//-----------------------------------------------------------------------------
void Polygon::computeCenter()
//-----------------------------------------------------------------------------
{
  center = 0.0;
  for (int i = 0; i < points; i++)
    center += vertices[i];
  center /= points;
}

//-----------------------------------------------------------------------------
void Polygon::resetCoordinates()
//-----------------------------------------------------------------------------
{
  center = 0.0;
  for (int i = 0; i < points; i++)
  {
    vertices[i] = nodes[i]->coordinates;
    center += vertices[i];
  }
  center /= points;
}

//-----------------------------------------------------------------------------
void Polygon::remapVertices(Vec3D center, Vec3D worldCenter, Vec3D worldScale)
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < points; i++)
    vertices[i] = worldScale.componentsProduct(vertices[i] - center) + worldCenter;
}

//-----------------------------------------------------------------------------
String Polygon::getWhitePolygonSvgCode()
//-----------------------------------------------------------------------------
{
  String svgcode = "";
  String crd1, crd2;
  Vec3D point;
  svgcode += "<polygon\n";
  svgcode += " stroke=\"black\"\n";
  svgcode += " fill=\"white\"\n";
  svgcode += " stroke-width=\"1\"\n";
  svgcode += " stroke-linejoin=\"round\"\n";
  svgcode += " points=\"";

  for (int i = 0; i < points; i++)
  {
    point = getVertex(i);
    crd1.convert(point(0));
    crd2.convert(point(1));
    svgcode += crd1 + "," + crd2 + " ";
  }
  svgcode += "\" />\n";
  return svgcode;
}

//-----------------------------------------------------------------------------
void Polygon::rotate(Tensor2 Mat)
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < points; i++)
    vertices[i] = Mat * vertices[i];
}