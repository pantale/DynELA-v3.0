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
  friend bool compareCentersXYZ(Polygon *p1, Polygon *p2);
  friend bool compareCentersYZX(Polygon *p1, Polygon *p2);
  friend bool compareCentersZXY(Polygon *p1, Polygon *p2);
  friend bool zBufferCenters(Polygon *p1, Polygon *p2);

private:
  bool visible = true;
  int points = 0;
  Node *nodes[PolygonMaxNodes];
  Vec3D center;
  Vec3D normal;
  Vec3D vertices[PolygonMaxNodes];

public:
  // constructeurs
  Polygon();
  ~Polygon();

  bool isVisible() { return visible; }
  int numberOfPoints();
  String getFlatPolygonSvgCode(ColorMap &map, short field, bool stroke = true, int width = 1);
  String getInterpolatedPolygonSvgCode(ColorMap &map, short field, bool stroke = true, int width = 1);
  String getWhitePolygonSvgCode();
  Vec3D getVertex(int);
  void add(Node *node);
  void add(Vec3D point);
  void computeCenter();
  void computeNormal();
  void init();
  void remapVertices(Vec3D center, Vec3D worldCenter, Vec3D worldScale);
  void resetCoordinates();
  void rotate(Tensor2 Mat);
};

class PolygonPatch
{
public:
  double value;
  int colorIndex;
  int number;
  Vec3D coords[8];

public:
  // constructeurs
  PolygonPatch() {}
  ~PolygonPatch() {}
};

class PolygonPatches
{
private:
  PolygonPatch *createPolygonPatch();
  void createSubPatch(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map);
  void createSubPatch2(int points, Vec3D *coords, double *valuesR, int *valuesI, ColorMap &map);
  void reorderPoints(Vec3D *coords, int cur);

public:
  int decompLevel;
  List<PolygonPatch *> patches;

public:
  // constructeurs
  PolygonPatches();
  ~PolygonPatches() {}
  void createPatch(Polygon *polygon, ColorMap &map, short field);
};

#endif
