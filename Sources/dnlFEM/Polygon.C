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
#include <Field.h>
#include <DynELA.h>

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
  String tmp1, tmp2;
  Vec3D point;

  // Begin polygon
  svgcode += "<polygon\n";
  svgcode += " stroke=\"black\"\n";
  svgcode += " fill=\"white\"\n";
  svgcode += " stroke-width=\"1\"\n";
  svgcode += " stroke-linejoin=\"round\"\n";
  svgcode += " points=\"";

  // polygon points
  for (int i = 0; i < points; i++)
  {
    point = getVertex(i);
    tmp1.convert(point(0));
    tmp2.convert(point(1));
    svgcode += tmp1 + "," + tmp2 + " ";
  }

  // End polygon
  svgcode += "\" />\n";

  // Returns the string
  return svgcode;
}

//-----------------------------------------------------------------------------
String Polygon::getInterpolatedPolygonSvgCode(ColorMap &map, short field, bool stroke, int width)
//-----------------------------------------------------------------------------
{
  String svgcode = "";
  String tmp1, tmp2;
  Vec3D point;
  String tmpWidth;
  tmpWidth.convert(width);

  double val = 0;
  for (int i = 0; i < points; i++)
  {
    val += nodes[i]->getNodalValue(field);
  }
  val /= points;

  String col = map.getStringColor(val, true);

  // Begin polygon
  svgcode += "<polygon\n";
  svgcode += " fill=\"" + col + "\"\n";
  if (stroke)
  {
    svgcode += " stroke=\"black\"\n";
    svgcode += " stroke-width=\"" + tmpWidth + "\"\n";
  }
  else
  {
    svgcode += " stroke=\"" + col + "\"\n";
    svgcode += " stroke-width=\"0.25\"\n";
  }
  svgcode += " stroke-linejoin=\"round\"\n";

  svgcode += " points=\"";

  // polygon points
  for (int i = 0; i < points; i++)
  {
    point = getVertex(i);
    tmp1.convert(point(0));
    tmp2.convert(point(1));
    svgcode += tmp1 + "," + tmp2 + " ";
  }

  // End polygon
  svgcode += "\" />\n";

  // Returns the string
  return svgcode;
}

//-----------------------------------------------------------------------------
void Polygon::rotate(Tensor2 Mat)
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < points; i++)
    vertices[i] = Mat * vertices[i];
}