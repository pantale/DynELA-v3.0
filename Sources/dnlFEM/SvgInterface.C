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

#include <SvgInterface.h>
#include <List.h>
#include <DynELA.h>
#include <Node.h>
#include <Element.h>
#include <Field.h>
#include <Model.h>

//-----------------------------------------------------------------------------
SvgInterface::SvgInterface(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;
}

//-----------------------------------------------------------------------------
SvgInterface::SvgInterface(const SvgInterface &SvgInterface)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
SvgInterface::~SvgInterface()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void SvgInterface::initSvgFile(String fileName)
//-----------------------------------------------------------------------------
{
  if (fileName == "")
  {
    fatalError("SvgInterface::init", "Must specify a log filename in the constructor");
  }

  // put the name
  _fileName = fileName;

  // open the stream
  _stream.open(_fileName.chars(), std::fstream::out);

  if (!_stream.is_open())
  {
    fatalError("SvgInterface::init", "Cannot open _stream for file %s", _fileName.chars());
  }
}

//-----------------------------------------------------------------------------
void SvgInterface::closeSvgFile()
//-----------------------------------------------------------------------------
{
  // close the stream
  _stream.close();
}

//-----------------------------------------------------------------------------
void SvgInterface::headerWrite()
//-----------------------------------------------------------------------------
{
  _stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
  _stream << "<svg\n";
  _stream << " _width=\"" << _width << "cm\"\n";
  _stream << " _height=\"" << _height << "cm\"\n";
  _stream << " viewBox=\"" << _svgBottomLeft(0) << " " << _svgBottomLeft(1) << " " << _svgTopRight(0) << " " << _svgTopRight(1) << "\"\n";
  _stream << " version=\"1.1\">\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::tailWrite()
//-----------------------------------------------------------------------------
{
  _stream << "</svg>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::initDrawing()
//-----------------------------------------------------------------------------
{
  // Initialize polygons
  if (!_initialized)
  {
    dynelaData->drawing.initPolygons();
    _initialized = true;
  }
  else
  {
    dynelaData->drawing.resetPolygons();
  }

  if (_rotate)
    dynelaData->drawing.rotate(_axis, _angle);

  // Compute Bound Box of the model
  dynelaData->drawing.computeBoundBox();

  // Compute _scale factor
  Vec3D delta = dynelaData->drawing.topRight - dynelaData->drawing.bottomLeft;
  Vec3D svgDelta = _svgTopRight - _svgBottomLeft;
  _scale = _scaleRatio * dnlMin(svgDelta(0), svgDelta(1)) / dnlMax(delta(0), delta(1));

  // Compute center
  _svgCenter = (_svgTopRight + _svgBottomLeft) / 2;
  dynelaData->drawing.worldCenter = _svgCenter;

  dynelaData->drawing.worldScale(0) = _scale;
  dynelaData->drawing.worldScale(1) = -_scale;
  dynelaData->drawing.worldScale(2) = 0;

  // Remap polygons to fit page
  dynelaData->drawing.mapToWorld();
}

//-----------------------------------------------------------------------------
void SvgInterface::rotate(Vec3D axis, double angle)
//-----------------------------------------------------------------------------
{
  _rotate = true;
  _axis = axis;
  _angle = angle;
}

//-----------------------------------------------------------------------------
void SvgInterface::meshWrite()
//-----------------------------------------------------------------------------
{
  _stream << "<g id =\"mesh\">\n";

  Polygon *polygon = dynelaData->drawing.polygons.first();
  while ((polygon = dynelaData->drawing.polygons.currentUp()) != NULL)
  {
    _stream << polygon->getWhitePolygonSvgCode();
  }
  _stream << "</g>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::flatPolygonsWrite()
//-----------------------------------------------------------------------------
{
  _stream << "<g id =\"field\">\n";

  Polygon *polygon = dynelaData->drawing.polygons.first();
  while ((polygon = dynelaData->drawing.polygons.currentUp()) != NULL)
  {
    _stream << polygon->getFlatPolygonSvgCode(colorMap, field);
  }
  _stream << "</g>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::interpolatedPolygonsWrite()
//-----------------------------------------------------------------------------
{
  _stream << "<g id =\"field\">\n";

  Polygon *polygon = dynelaData->drawing.polygons.first();
  while ((polygon = dynelaData->drawing.polygons.currentUp()) != NULL)
  {
    _stream << "<g>\n";
    _stream << polygon->getInterpolatedPolygonSvgCode(colorMap, field);
    _stream << "</g>\n";
  }
  _stream << "</g>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::textWrite(Vec3D location, String text, int size, String color)
//-----------------------------------------------------------------------------
{
  _stream << "<text \n";
  _stream << " x=\"" << location(0) << "\" y=\"" << location(1) << "\"\n";
  _stream << " font-family=\"Ubuntu\"\n";
  _stream << " font-size=\"" << size << "\"\n";
  _stream << " fill=\"" << color << "\" >\n";
  _stream << text;
  _stream << "</text>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::filledRectangleWrite(int x1, int y1, int x2, int y2, String col)
//-----------------------------------------------------------------------------
{
  _stream << "<polygon\n";
  _stream << " fill=\"" + col + "\"\n";
  _stream << " points=\"";
  _stream << x1 << "," << y1 << " ";
  _stream << x1 << "," << y2 << " ";
  _stream << x2 << "," << y2 << " ";
  _stream << x2 << "," << y1 << " ";
  _stream << "\" />\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::rectangleWrite(int x1, int y1, int x2, int y2, String col, int width)
//-----------------------------------------------------------------------------
{
  _stream << "<polygon\n";
  _stream << " stroke=\"" + col + "\"\n";
  _stream << " stroke-width=\"" << width << "\"\n";
  _stream << " stroke-linejoin=\"round\"\n";
  _stream << " fill=\"none\" \n";
  _stream << " points=\"";
  _stream << x1 << "," << y1 << " ";
  _stream << x1 << "," << y2 << " ";
  _stream << x2 << "," << y2 << " ";
  _stream << x2 << "," << y1 << " ";
  _stream << "\" />\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::lineWrite(int x1, int y1, int x2, int y2, int width)
//-----------------------------------------------------------------------------
{
  _stream << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" stroke=\"black\" stroke-width=\"" << width << "\"/>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::legendWrite()
//-----------------------------------------------------------------------------
{
  int _height = 25 * colorMap.getLevels();
  int _width = 50;
  int offx = 50;
  int offy = 120;
  int offbars = 20;
  int levels;
  double percent;
  String col;
  double min, max;
  double bx, by, tx, ty;
  String textVal;
  Field ffield;

  colorMap.getBounds(min, max, levels);
  int dy = int(_height / levels);

  _stream << "<g id =\"legend\">\n";

  for (int level = 0; level < levels; level++)
  {
    percent = double(level) / (levels - 1);
    col = colorMap.getStringColor(min + (max - min) * percent, true);
    bx = offx;
    by = (_height + offy) - level * dy;
    tx = offx + _width;
    ty = (_height + offy) - (level + 1) * dy;
    filledRectangleWrite(bx, by, tx, ty, col);
  }
  rectangleWrite(offx, offy, offx + _width, offy + _height);

  for (int level = 0; level < levels + 1; level++)
  {
    percent = double(level) / (levels);
    bx = offx;
    by = (_height + offy) - level * dy;

    lineWrite(bx, by, bx + _width + offbars, by);
    textVal.convert(min + (max - min) * percent, "%10.3E");
    textWrite(Vec3D(bx + _width + offbars + 5, by + 7, 0), textVal, 20);
  }
  filledRectangleWrite(offx - 20, offy - 90, offx + _width + 140, offy - 20, "#C0C0C0");
  textWrite(Vec3D(offx, offy - 60, 0), ffield.getVtklabel(field), 26);
  rectangleWrite(offx - 20, offy - 90, offx + _width + 140, offy + _height + 20, "black", 4);
  lineWrite(offx - 20, offy - 20, offx + _width + 140, offy - 20, 4);

  textVal.convert(dynelaData->model->currentTime, "%10.3E");
  textVal = "time:" + textVal;
  textWrite(Vec3D(offx, offy - 30, 0), textVal, 24);

  _stream << "</g>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::write(String fileName, short _field)
//-----------------------------------------------------------------------------
{
  field = _field;

  initDrawing();

  initSvgFile(fileName);

  // Header write
  headerWrite();

  // Writes a mesh();
  if (field == -1)
  {
    meshWrite();
  }
  else
  {
    // Get bounds values for field
    double max, min;
    dynelaData->getNodalValuesRange(field, min, max);

    // Create colorMap
    colorMap.setBounds(min, max);
    field = field;

    interpolatedPolygonsWrite();
    //flatPolygonsWrite();

    legendWrite();
  }

  // Wites the title of application
  textWrite(Vec3D(50, 1550, 0), "DynELA FEM code v.3.0", 40);

  // Tail write
  tailWrite();

  closeSvgFile();
}
