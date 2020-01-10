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

class SvgInterface
{
private:
  std::ofstream _stream;
  String _fileName;
  double width = 16;
  double height = 16;
  double scale = 1;
  double scaleRatio = 0.95;
  const Vec3D svgBottomLeft = Vec3D(0, 0, 0);
  const Vec3D svgTopRight = Vec3D(1600, 1600, 0);
  Vec3D svgCenter;
  Polygon polygon;

public:
  String name = "_noname_"; //!< Name of the VTK interface

  // constructeurs
  SvgInterface(char *newName = NULL);
  SvgInterface(const SvgInterface &SvgInterface);
  ~SvgInterface();

  void init(String fileName);
  void close();
  void write();
  void rotate(Vec3D axis, double angle);
  void initDrawing();

#ifndef SWIG
  void headerWrite();
  void tailWrite();
  void polygonsWrite();
  void textWrite(Vec3D location, String text, int size = 30, String color = "black");
#endif
};

#endif
