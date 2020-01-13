/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Drawing.h
  \brief Declaration file for the Drawing class

  This file is the declaration file for the Drawing class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <Drawing.h>
#include <DynELA.h>
#include <Element.h>

//-----------------------------------------------------------------------------
Drawing::Drawing()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Drawing::Drawing(const Drawing &Drawing)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Drawing::~Drawing()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Drawing::computeBoundBox()
//-----------------------------------------------------------------------------
{
  Vec3D coordinates = polygons.first()->vertices[0];
  bottomLeft = topRight = coordinates;

  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    for (int i = 0; i < polygon->points; i++)
    {
      coordinates = polygon->vertices[i];
      for (int j = 0; j < 3; j++)
      {
        if (coordinates(j) < bottomLeft(j))
          bottomLeft(j) = coordinates(j);
        if (coordinates(j) > topRight(j))
          topRight(j) = coordinates(j);
      }
    }
  }

  center = (topRight + bottomLeft) / 2;
}

//-----------------------------------------------------------------------------
void Drawing::initPolygons()
//-----------------------------------------------------------------------------
{
  Element *pel = dynelaData->elements.first();
  while ((pel = dynelaData->elements.currentUp()) != NULL)
  {
    for (int face = 0; face < pel->getNumberOfFaces(); face++)
    {
      // Creates a new polygon
      Polygon *polygon = new Polygon;

      // Add vertices
      for (int node = 0; node < pel->getNumberOfNodesOnFace(face); node++)
      {
        polygon->add(pel->getNodeOnFace(face, node));
      }

      // Computes center of polygon
      polygon->computeCenter();

      // Add polygon to list
      polygons << polygon;
    }
  }
}

//-----------------------------------------------------------------------------
void Drawing::resetPolygons()
//-----------------------------------------------------------------------------
{
  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    polygon->resetCoordinates();
  }
}

//-----------------------------------------------------------------------------
void Drawing::rotate(Vec3D axis, double angle)
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

  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    polygon->rotate(Mat);
  }
}

//-----------------------------------------------------------------------------
void Drawing::mapToWorld()
//-----------------------------------------------------------------------------
{
  Polygon *polygon = polygons.first();
  while ((polygon = polygons.currentUp()) != NULL)
  {
    polygon->remapVertices(center, worldCenter, worldScale);
  }
}