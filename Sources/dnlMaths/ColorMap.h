/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file ColorMap.h
  \brief Declaration of the ColorMap class.

  This file declares the ColorMap class used to produce svg outputs.
  \ingroup dnlMaths
*/

#ifndef __dnlMaths_ColorMap_h__
#define __dnlMaths_ColorMap_h__

#include <Vec3D.h>
#include <DiscreteFunction.h>

/*! 
  \brief Manipulation of a color map for SVG files

This class is used to store and manipulate information about color maps for the DynELA Finite Element code.
  \ingroup dnlMaths
*/
class ColorMap
{
private:
#ifndef SWIG
  enum
  {
    DeepCM,    //!< Definition of a deep colormap
    RDeepCM,   //!< Definition of a reverse deep colormap
    NormalCM,  //!< Definition of a standard colormap
    RNormalCM, //!< Definition of a reverse standard colormap
    GrayCM,    //!< Definition of a gray colormap
    RGrayCM    //!< Definition of a reverse gray colormap
  };
  Vec3D BLACK = Vec3D(0, 0, 0);      //!< Black color definition
  Vec3D BLUE = Vec3D(0, 0, 1);       //!< Blue color definition
  Vec3D CYAN = Vec3D(0, 1, 1);       //!< Cyan color definition
  Vec3D DARKBLUE = Vec3D(0, 0, 0.5); //!< Darkblue color definition
  Vec3D DARKRED = Vec3D(0.5, 0, 0);  //!< Darkred color definition
  Vec3D GRAY = Vec3D(0.5, 0.5, 0.5); //!< Gray color definition
  Vec3D GREEN = Vec3D(0, 1, 0);      //!< Green color definition
  Vec3D RED = Vec3D(1, 0, 0);        //!< Red color definition
  Vec3D WHITE = Vec3D(1, 1, 1);      //!< White color definition
  Vec3D YELLOW = Vec3D(1, 1, 0);     //!< Yellow color definition
#endif
  DiscreteFunction _blueComponent;  //!< Discrete function associated to the blue channel
  DiscreteFunction _greenComponent; //!< Discrete function associated to the green channel
  DiscreteFunction _redComponent;   //!< Discrete function associated to the red channel
  double _min;                      //!< min of the colors
  double _max;                      //!< max of the colors
  int _levels = 16;                 //!< Number of levels
  short _currentColorMap = DeepCM;  //!< Current color map
  void clearColorMap();
  void resetColorMap();

public:
  ColorMap();
  ~ColorMap();

  double getMax();
  double getMin();
  int getIntColor(double value);
  int getLevels();
  String getStringColor(double value, bool steps = false);
  Vec3D getVec3DColor(double value, bool steps = false);
  void getBounds(double &min, double &max, int &levels);
  void setBounds(double min, double max);
  void setColorMap();
  void setDeepColorMap();
  void setGrayMap();
  void setLevels(int levels);
  void setReverseColorMap();
  void setReverseDeepColorMap();
  void setReverseGrayMap();
};

#endif
