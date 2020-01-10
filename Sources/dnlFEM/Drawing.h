/***************************************************************************
 *                                                                         *
 *  DynELA FcomputeBoundBoxe Element Code v 3.0                                       *
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

#ifndef __dnlFEM_Drawing_h__
#define __dnlFEM_Drawing_h__

#include <String.h>
#include <Vec3D.h>
#include <Polygon.h>

class Drawing
{
private:
  bool initialized = false;

public:
  List<Polygon *> polygons;
  Vec3D bottomLeft, topRight;
  Vec3D center;
  Vec3D worldCenter;
  Vec3D worldScale;

public:
  // constructeurs
  Drawing();
  Drawing(const Drawing &Drawing);
  ~Drawing();

  void computeBoundBox();
  void initPolygons();
  void mapToWorld();
  void rotate(Vec3D axis, double angle);
  void resetPolygons();
};

#endif
