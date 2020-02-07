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

#include <ColorMap.h>

//-----------------------------------------------------------------------------
ColorMap::ColorMap()
//-----------------------------------------------------------------------------
{
  // Set deep color map by default
  setDeepColorMap();
}

//-----------------------------------------------------------------------------
void ColorMap::clearMap()
//-----------------------------------------------------------------------------
{
  _redComponent.flush();
  _greenComponent.flush();
  _blueComponent.flush();
}

//-----------------------------------------------------------------------------
void ColorMap::setColorMap()
//-----------------------------------------------------------------------------
{
  clearMap();
  double last = double(_levels - 1) / _levels;
  CM = NormalCM;

  // 0.00 - Blue color
  _redComponent.add(0.00, BLUE(0));
  _greenComponent.add(0.00, BLUE(1));
  _blueComponent.add(0.00, BLUE(2));

  // 0.25 - Cyan color
  _redComponent.add(last / 4, CYAN(0));
  _greenComponent.add(last / 4, CYAN(1));
  _blueComponent.add(last / 4, CYAN(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(3 * last / 4, YELLOW(0));
  _greenComponent.add(3 * last / 4, YELLOW(1));
  _blueComponent.add(3 * last / 4, YELLOW(2));

  // 1.00 - Red color
  _redComponent.add(last, RED(0));
  _greenComponent.add(last, RED(1));
  _blueComponent.add(last, RED(2));

  // 1.00 - Red color
  _redComponent.add(1.00, RED(0));
  _greenComponent.add(1.00, RED(1));
  _blueComponent.add(1.00, RED(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setReverseDeepColorMap()
//-----------------------------------------------------------------------------
{
  clearMap();
  double last = double(_levels - 1) / _levels;
  CM = RDeepCM;

  // 0.00 - Dark Blue color
  _redComponent.add(0, DARKRED(0));
  _greenComponent.add(0, DARKRED(1));
  _blueComponent.add(0, DARKRED(2));

  // 0.00 - Blue color
  _redComponent.add(last / 6, RED(0));
  _greenComponent.add(last / 6, RED(1));
  _blueComponent.add(last / 6, RED(2));

  // 0.25 - Cyan color
  _redComponent.add(2 * last / 6, YELLOW(0));
  _greenComponent.add(2 * last / 6, YELLOW(1));
  _blueComponent.add(2 * last / 6, YELLOW(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(2 * last / 3, CYAN(0));
  _greenComponent.add(2 * last / 3, CYAN(1));
  _blueComponent.add(2 * last / 3, CYAN(2));

  // 1.00 - Red color
  _redComponent.add(5 * last / 6, BLUE(0));
  _greenComponent.add(5 * last / 6, BLUE(1));
  _blueComponent.add(5 * last / 6, BLUE(2));

  // 1.00 - Red color
  _redComponent.add(last, DARKBLUE(0));
  _greenComponent.add(last, DARKBLUE(1));
  _blueComponent.add(last, DARKBLUE(2));

  // 1.00 - Red color
  _redComponent.add(1.00, DARKBLUE(0));
  _greenComponent.add(1.00, DARKBLUE(1));
  _blueComponent.add(1.00, DARKBLUE(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setDeepColorMap()
//-----------------------------------------------------------------------------
{
  clearMap();
  double last = double(_levels - 1) / _levels;
  CM = DeepCM;

  // 0.00 - Dark Blue color
  _redComponent.add(0, DARKBLUE(0));
  _greenComponent.add(0, DARKBLUE(1));
  _blueComponent.add(0, DARKBLUE(2));

  // 0.00 - Blue color
  _redComponent.add(last / 6, BLUE(0));
  _greenComponent.add(last / 6, BLUE(1));
  _blueComponent.add(last / 6, BLUE(2));

  // 0.25 - Cyan color
  _redComponent.add(2 * last / 6, CYAN(0));
  _greenComponent.add(2 * last / 6, CYAN(1));
  _blueComponent.add(2 * last / 6, CYAN(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(2 * last / 3, YELLOW(0));
  _greenComponent.add(2 * last / 3, YELLOW(1));
  _blueComponent.add(2 * last / 3, YELLOW(2));

  // 1.00 - Red color
  _redComponent.add(5 * last / 6, RED(0));
  _greenComponent.add(5 * last / 6, RED(1));
  _blueComponent.add(5 * last / 6, RED(2));

  // 1.00 - Red color
  _redComponent.add(last, DARKRED(0));
  _greenComponent.add(last, DARKRED(1));
  _blueComponent.add(last, DARKRED(2));

  // 1.00 - Red color
  _redComponent.add(1.00, DARKRED(0));
  _greenComponent.add(1.00, DARKRED(1));
  _blueComponent.add(1.00, DARKRED(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setReverseColorMap()
//-----------------------------------------------------------------------------
{
  clearMap();
  double last = double(_levels - 1) / _levels;
  CM = RNormalCM;

  // 0.00 - Blue color
  _redComponent.add(0, RED(0));
  _greenComponent.add(0, RED(1));
  _blueComponent.add(0, RED(2));

  // 0.25 - Cyan color
  _redComponent.add(last / 4, YELLOW(0));
  _greenComponent.add(last / 4, YELLOW(1));
  _blueComponent.add(last / 4, YELLOW(2));

  // 0.50 - Green color
  _redComponent.add(last / 2, GREEN(0));
  _greenComponent.add(last / 2, GREEN(1));
  _blueComponent.add(last / 2, GREEN(2));

  // 0.75 - Yellow color
  _redComponent.add(3 * last / 4, CYAN(0));
  _greenComponent.add(3 * last / 4, CYAN(1));
  _blueComponent.add(3 * last / 4, CYAN(2));

  // 1.00 - Red color
  _redComponent.add(last, BLUE(0));
  _greenComponent.add(last, BLUE(1));
  _blueComponent.add(last, BLUE(2));

  // 1.00 - Red color
  _redComponent.add(1.00, BLUE(0));
  _greenComponent.add(1.00, BLUE(1));
  _blueComponent.add(1.00, BLUE(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setGrayMap()
//-----------------------------------------------------------------------------
{
  clearMap();
  double last = double(_levels - 1) / _levels;
  CM = GrayCM;

  // 0.00 - Blue color
  _redComponent.add(0, BLACK(0));
  _greenComponent.add(0, BLACK(1));
  _blueComponent.add(0, BLACK(2));

  _redComponent.add(last, WHITE(0));
  _greenComponent.add(last, WHITE(1));
  _blueComponent.add(last, WHITE(2));

  _redComponent.add(1.00, WHITE(0));
  _greenComponent.add(1.00, WHITE(1));
  _blueComponent.add(1.00, WHITE(2));
}

//-----------------------------------------------------------------------------
void ColorMap::setReverseGrayMap()
//-----------------------------------------------------------------------------
{
  clearMap();
  double last = double(_levels - 1) / _levels;
  CM = RGrayCM;

  // 0.00 - Blue color
  _redComponent.add(0, WHITE(0));
  _greenComponent.add(0, WHITE(1));
  _blueComponent.add(0, WHITE(2));

  // 1.00 - Red color
  _redComponent.add(last, BLACK(0));
  _greenComponent.add(last, BLACK(1));
  _blueComponent.add(last, BLACK(2));

  // 1.00 - Red color
  _redComponent.add(1.00, BLACK(0));
  _greenComponent.add(1.00, BLACK(1));
  _blueComponent.add(1.00, BLACK(2));
}

//-----------------------------------------------------------------------------
void ColorMap::resetMap()
//-----------------------------------------------------------------------------
{
  switch (CM)
  {
  case DeepCM:
    setDeepColorMap();
    break;
  case RDeepCM:
    setReverseDeepColorMap();
    break;
  case NormalCM:
    setColorMap();
    break;
  case RNormalCM:
    setReverseColorMap();
    break;
  case GrayCM:
    setGrayMap();
    break;
  case RGrayCM:
    setReverseGrayMap();
    break;
  }
}

//-----------------------------------------------------------------------------
void ColorMap::setBounds(double min, double max)
//-----------------------------------------------------------------------------
{
  _min = min;
  _max = max;
}

//-----------------------------------------------------------------------------
void ColorMap::setLevels(int levels)
//-----------------------------------------------------------------------------
{
  _levels = levels;
  resetMap();
}

//-----------------------------------------------------------------------------
int ColorMap::getIntColor(double value)
//-----------------------------------------------------------------------------
{
  // If less than min
  if (value < _min)
    return -1;

  // If more than max
  if (value > _max)
    return -1;

  if (_max == _min)
    return 0;

  if (value == _max)
    return _levels - 1;

  // Compute color using palette
  return int((value - _min) / (_max - _min) * _levels);
}

//-----------------------------------------------------------------------------
Vec3D ColorMap::getVec3DColor(double value, bool steps)
//-----------------------------------------------------------------------------
{
  if (_max - _min < 1e-8)
    return GRAY;

  // If less than min
  if (value < _min)
    return BLACK;

  // If more than max
  if (value > _max)
    return WHITE;

  if (value == _max)
    return Vec3D(_redComponent.getValue(1), _greenComponent.getValue(1), _blueComponent.getValue(1));

  // Compute color using palette
  double val = (value - _min) / (_max - _min);

  if (steps)
    val = double(int(val * _levels)) / _levels;

  return Vec3D(_redComponent.getValue(val), _greenComponent.getValue(val), _blueComponent.getValue(val));
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