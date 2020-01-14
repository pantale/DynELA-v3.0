/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file SvgInterface.h
  \brief Declaration file for the SvgInterface class

  This file is the declaration file for the SvgInterface class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlFEM_SvgInterface_h__
#define __dnlFEM_SvgInterface_h__

#include <String.h>
#include <List.h>
#include <iostream>
#include <fstream>
#include <Vec3D.h>
#include <Polygon.h>
#include <ColorMap.h>

class SvgInterface
{
private:
  std::ofstream _stream;
  String _fileName;
  double _width = 16;
  double _height = 16;
  double _scale = 1;
  double _scaleRatio = 0.95;
  const Vec3D _svgBottomLeft = Vec3D(0, 0, 0);
  const Vec3D _svgTopRight = Vec3D(1600, 1600, 0);
  Vec3D _svgCenter;
  bool _initialized = false;
  bool _rotate = false;
  Vec3D _axis;
  double _angle;
#ifndef SWIG
  void initSvgFile(String fileName);
  void closeSvgFile();
  void headerWrite();
  void tailWrite();
  void meshWrite();
  void interpolatedPolygonsWrite();
  void flatPolygonsWrite();
  void textWrite(Vec3D location, String text, int size = 30, String color = "black");
  void rectangleWrite(int x1, int y1, int x2, int y2, String col = "black", int width = 1);
  void filledRectangleWrite(int x1, int y1, int x2, int y2, String col);
  void legendWrite();
  void lineWrite(int x1, int y1, int x2, int y2, int width = 1);

#endif

public:
  String name = "_noname_"; //!< Name of the VTK interface
  ColorMap colorMap;
  short field;

  // constructeurs
  SvgInterface(char *newName = NULL);
  SvgInterface(const SvgInterface &SvgInterface);
  ~SvgInterface();

  void rotate(Vec3D axis, double angle);
  void write(String fileName, short field = -1);
  void initDrawing();
};

#endif
