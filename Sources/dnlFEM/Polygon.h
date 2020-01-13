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

#ifndef __dnlFEM_Polygon_h__
#define __dnlFEM_Polygon_h__

#include <String.h>
#include <List.h>
#include <iostream>
#include <fstream>
#include <Vec3D.h>
#include <Tensor2.h>
#include <Node.h>
#include <ColorMap.h>

class Polygon
{
  friend class Drawing;

private:
  Node *nodes[8];
  Vec3D vertices[8];
  Vec3D center;
  int points = 0;

public:
  // constructeurs
  Polygon();
  ~Polygon();

  void init();
  int numberOfPoints();
  void add(Node *node);
  void add(Vec3D point);
  Vec3D getVertex(int);
  String getWhitePolygonSvgCode();
  String getInterpolatedPolygonSvgCode(ColorMap &map, short field, bool stroke = true, int width = 1);
  void computeCenter();
  void remapVertices(Vec3D center, Vec3D worldCenter, Vec3D worldScale);
  void rotate(Tensor2 Mat);
  void resetCoordinates();
};

#endif
