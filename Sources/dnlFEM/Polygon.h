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

#define PolygonMaxNodes 8

class Polygon
{
  friend class Drawing;
  friend class PolygonPatches;

private:
  Node *nodes[PolygonMaxNodes];
  Vec3D vertices[PolygonMaxNodes];
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
  String getFlatPolygonSvgCode(ColorMap &map, short field, bool stroke = true, int width = 1);
  String getInterpolatedPolygonSvgCode(ColorMap &map, short field, bool stroke = true, int width = 1);
  void computeCenter();
  void remapVertices(Vec3D center, Vec3D worldCenter, Vec3D worldScale);
  void rotate(Tensor2 Mat);
  void resetCoordinates();
};

class PolygonPatch
{
public:
  Vec3D coords[8];
  int colorIndex;
  double value;
  int number;

public:
  // constructeurs
  PolygonPatch() {}
  ~PolygonPatch() {}
};

class PolygonPatches
{
private:
  void createSubPatch(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map);
  void createSubPatch2(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map);
  PolygonPatch *createPolygonPatch();
  void reorderPoints(Vec3D *coords, int cur);

public:
  List<PolygonPatch *> patches;
  int decompLevel;

public:
  // constructeurs
  PolygonPatches();
  ~PolygonPatches() {}
  void createPatch(Polygon *polygon, ColorMap &map, short field);
};

#endif
