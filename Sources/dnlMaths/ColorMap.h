/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file ColorMap.h
  \brief Definition of the ColorMap class and OS operators.

  This file defines all the OS operators in the ColorMap class.
  \ingroup dnlKernel
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlMaths_ColorMap_h__
#define __dnlMaths_ColorMap_h__

#include <Vec3D.h>
#include <DiscreteFunction.h>

class ColorMap
{
private:
#ifndef SWIG
  Vec3D DARKRED = Vec3D(0.5, 0, 0);
  Vec3D RED = Vec3D(1, 0, 0);
  Vec3D GREEN = Vec3D(0, 1, 0);
  Vec3D BLUE = Vec3D(0, 0, 1);
  Vec3D DARKBLUE = Vec3D(0, 0, 0.5);
  Vec3D WHITE = Vec3D(1, 1, 1);
  Vec3D BLACK = Vec3D(0, 0, 0);
  Vec3D YELLOW = Vec3D(1, 1, 0);
  Vec3D CYAN = Vec3D(0, 1, 1);
  Vec3D GRAY = Vec3D(0.5, 0.5, 0.5);
#endif
  double _min, _max;
  int _levels = 16;
  DiscreteFunction red_func;
  DiscreteFunction green_func;
  DiscreteFunction blue_func;
  void clearMap();

public:
  ColorMap();
  ~ColorMap() {}
  void setColorMap();
  void setReverseColorMap();
  void setDeepColorMap();
  void setReverseDeepColorMap();
  void setGrayMap();
  void setReverseGrayMap();
  void setBounds(double min, double max);
  void setLevels(int levels);
  Vec3D getVec3DColor(double value, bool steps = false);
  String getStringColor(double value, bool steps = false);
  int getIntColor(double value);
  void getBounds(double &min, double &max, int &levels);
  double getMin() { return _min; }
  double getMax() { return _max; }
  int getLevels() { return _levels; }
};

#endif
