/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file ColorMap.C
  \brief Fichier .C de definitions associees e l'OS ColorMap
  \ingroup basicTools

  Ce fichier regroupe la definition de la classe ColorMap utilisee pour l'environnement de travail ColorMap.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include "ColorMap.h"

//-----------------------------------------------------------------------------
ColorMap::ColorMap()
//-----------------------------------------------------------------------------
{
  // Set color map
  setColorMap();
}

//-----------------------------------------------------------------------------
void ColorMap::clearMap()
//-----------------------------------------------------------------------------
{
  red_func.flush();
  green_func.flush();
  blue_func.flush();
}

//-----------------------------------------------------------------------------
void ColorMap::setColorMap()
//-----------------------------------------------------------------------------
{
  clearMap();

  // 0.00 - Blue color
  red_func.add(0, BLUE(0));
  green_func.add(0, BLUE(1));
  blue_func.add(0, BLUE(2));

  // 0.25 - Cyan color
  red_func.add(0.25, CYAN(0));
  green_func.add(0.25, CYAN(1));
  blue_func.add(0.25, CYAN(2));

  // 0.50 - Green color
  red_func.add(0.50, GREEN(0));
  green_func.add(0.50, GREEN(1));
  blue_func.add(0.50, GREEN(2));

  // 0.75 - Yellow color
  red_func.add(0.75, YELLOW(0));
  green_func.add(0.75, YELLOW(1));
  blue_func.add(0.75, YELLOW(2));

  // 1.00 - Red color
  red_func.add(1.00, RED(0));
  green_func.add(1.00, RED(1));
  blue_func.add(1.00, RED(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setReverseColorMap()
//-----------------------------------------------------------------------------
{
  clearMap();

  // 0.00 - Blue color
  red_func.add(0, RED(0));
  green_func.add(0, RED(1));
  blue_func.add(0, RED(2));

  // 0.25 - Cyan color
  red_func.add(0.25, YELLOW(0));
  green_func.add(0.25, YELLOW(1));
  blue_func.add(0.25, YELLOW(2));

  // 0.50 - Green color
  red_func.add(0.50, GREEN(0));
  green_func.add(0.50, GREEN(1));
  blue_func.add(0.50, GREEN(2));

  // 0.75 - Yellow color
  red_func.add(0.75, CYAN(0));
  green_func.add(0.75, CYAN(1));
  blue_func.add(0.75, CYAN(2));

  // 1.00 - Red color
  red_func.add(1.00, BLUE(0));
  green_func.add(1.00, BLUE(1));
  blue_func.add(1.00, BLUE(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setGrayMap()
//-----------------------------------------------------------------------------
{
  clearMap();

  // 0.00 - Blue color
  red_func.add(0, BLACK(0));
  green_func.add(0, BLACK(1));
  blue_func.add(0, BLACK(2));

  red_func.add(1.00, WHITE(0));
  green_func.add(1.00, WHITE(1));
  blue_func.add(1.00, WHITE(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setReverseGrayMap()
//-----------------------------------------------------------------------------
{
  clearMap();

  // 0.00 - Blue color
  red_func.add(0, WHITE(0));
  green_func.add(0, WHITE(1));
  blue_func.add(0, WHITE(2));

  // 1.00 - Red color
  red_func.add(1.00, BLACK(0));
  green_func.add(1.00, BLACK(1));
  blue_func.add(1.00, BLACK(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setBounds(double min, double max, int levels)
//-----------------------------------------------------------------------------
{
  _min = min;
  _max = max;
  _levels = levels;
}

//-----------------------------------------------------------------------------
Vec3D ColorMap::getVec3DColor(double value, bool steps)
//-----------------------------------------------------------------------------
{
  // If less than min
  if (value < _min)
    return BLACK;

  // If more than max
  if (value > _max)
    return WHITE;

  if (_max == _min)
    return GRAY;

  // Compute color using palette
  double val = (value - _min) / (_max - _min);
  if (steps)
    val = double(int(val * _levels)) / _levels;
  return Vec3D(red_func.getValue(val), green_func.getValue(val), blue_func.getValue(val));
}

//-----------------------------------------------------------------------------
String ColorMap::getStringColor(double value, bool steps)
//-----------------------------------------------------------------------------
{
  char hex[3];
  Vec3D color = getVec3DColor(value, steps);
  String code = "#";

  sprintf(hex, "%02x", int(color(0) * 255));
  code += hex;
  sprintf(hex, "%02x", int(color(1) * 255));
  code += hex;
  sprintf(hex, "%02x", int(color(2) * 255));
  code += hex;
  return code;
}

//-----------------------------------------------------------------------------
void ColorMap::getBounds(double &min, double &max, int &levels)
//-----------------------------------------------------------------------------
{
  min = _min;
  max = _max;
  levels = _levels;
}