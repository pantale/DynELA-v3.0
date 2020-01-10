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
void SvgInterface::init(String fileName)
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

//  initData();
}

//-----------------------------------------------------------------------------
void SvgInterface::close()
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
  _stream << " width=\"" << width << "cm\"\n";
  _stream << " height=\"" << height << "cm\"\n";
  _stream << " viewBox=\"" << svgBottomLeft(0) << " " << svgBottomLeft(1) << " " << svgTopRight(0) << " " << svgTopRight(1) << "\"\n";
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
  dynelaData->drawing.initPolygons();

  // Compute Bound Box of the model
  dynelaData->drawing.computeBoundBox();

  // Compute scale factor
  Vec3D delta = dynelaData->drawing.topRight - dynelaData->drawing.bottomLeft;
  Vec3D svgDelta = svgTopRight - svgBottomLeft;
  scale = scaleRatio * dnlMin(svgDelta(0), svgDelta(1)) / dnlMax(delta(0), delta(1));

  // Compute center
  svgCenter = (svgTopRight + svgBottomLeft) / 2;
  dynelaData->drawing.worldCenter = svgCenter;

  dynelaData->drawing.worldScale(0) = scale;
  dynelaData->drawing.worldScale(1) = -scale;
  dynelaData->drawing.worldScale(2) = 0;

  // Remap polygons to fit page
  dynelaData->drawing.mapToWorld();
}

//-----------------------------------------------------------------------------
void SvgInterface::rotate(Vec3D axis, double angle)
//-----------------------------------------------------------------------------
{
  // Initialize polygons
  dynelaData->drawing.resetPolygons();

  dynelaData->drawing.rotate(axis, angle);
 
  // Compute Bound Box of the model
  dynelaData->drawing.computeBoundBox();

  // Compute scale factor
  Vec3D delta = dynelaData->drawing.topRight - dynelaData->drawing.bottomLeft;
  Vec3D svgDelta = svgTopRight - svgBottomLeft;
  scale = scaleRatio * dnlMin(svgDelta(0), svgDelta(1)) / dnlMax(delta(0), delta(1));

  // Compute center
  svgCenter = (svgTopRight + svgBottomLeft) / 2;
  dynelaData->drawing.worldCenter = svgCenter;

  dynelaData->drawing.worldScale(0) = scale;
  dynelaData->drawing.worldScale(1) = -scale;
  dynelaData->drawing.worldScale(2) = 0;

  // Remap polygons to fit page
  dynelaData->drawing.mapToWorld();
}
//-----------------------------------------------------------------------------
void SvgInterface::polygonsWrite()
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
void SvgInterface::textWrite(Vec3D location, String text, int size, String color)
//-----------------------------------------------------------------------------
{
  _stream << "<text \n";
  _stream << " x=\"" << location(0) << "\" y=\"" << location(1) << "\"\n";
  _stream << " font-family=\"Verdana\"\n";
  _stream << " font-size=\"" << size << "\"\n";
  _stream << " fill=\"" << color << "\" >\n";
  _stream << text;
  _stream << "</text>\n";
}

//-----------------------------------------------------------------------------
void SvgInterface::write()
//-----------------------------------------------------------------------------
{
  // Header write
  headerWrite();

  // Writes a mesh();
  polygonsWrite();

  // Wites the title of application
  textWrite(Vec3D(50, 1550, 0), "DynELA FEM code v.3.0", 40);

  // Tail write
  tailWrite();
}

/* //-----------------------------------------------------------------------------
void SvgInterface::nodesWrite()
//-----------------------------------------------------------------------------
{
  long nbNodes = dynelaData->nodes.getSize();
  _stream << "POINTS " << nbNodes << " float\n";

  for (long i = 0; i < nbNodes; i++)
    _stream << dynelaData->nodes(i)->coordinates(0) << " " << dynelaData->nodes(i)->coordinates(1) << " " << dynelaData->nodes(i)->coordinates(2) << "\n";

  _stream << "\n";
}
 */
/* //-----------------------------------------------------------------------------
void SvgInterface::elementsWrite()
//-----------------------------------------------------------------------------
{
  long nbElements = dynelaData->elements.getSize();
  long totNodes = 0;
  long nbNodes;
  Element *pElement;

  for (long i = 0; i < nbElements; i++)
    totNodes += dynelaData->elements(i)->nodes.getSize();
  totNodes += nbElements;

  _stream << "CELLS " << nbElements << " " << totNodes << "\n";

  for (long i = 0; i < nbElements; i++)
  {
    pElement = dynelaData->elements(i);
    nbNodes = pElement->nodes.getSize();
    _stream << nbNodes << " ";
    for (int i = 0; i < nbNodes; i++)
      _stream << pElement->nodes(i)->internalNumber() << " ";
    _stream << "\n";
  }

  _stream << "\n";

  _stream << "CELL_TYPES " << nbElements << "\n";
  for (long i = 0; i < nbElements; i++)
    _stream << dynelaData->elements(i)->getVtkType() << "\n";

  _stream << "\n";
}
 */
/* //-----------------------------------------------------------------------------
void SvgInterface::nodesNumbersWrite()
//-----------------------------------------------------------------------------
{
  long nbNodes = dynelaData->nodes.getSize();
  _stream << "SCALARS nodesNumbers" << nbNodes << " float\n";

  _stream << "\n";
}
 */
/* //-----------------------------------------------------------------------------
void SvgInterface::dataWrite()
//-----------------------------------------------------------------------------
{
  long nbNodes = dynelaData->nodes.getSize();
  short field;
  Field fields;
  bool lookupWriten = false;

  _stream << "POINT_DATA " << nbNodes << "\n";

  for (int i = 0; i < _outputFields.getSize(); i++)
  {
    field = _outputFields(i);

    // Scalar field
    if (fields.getType(field) == 0)
    {
      String _name = fields.getVtklabel(field);
      _stream << "SCALARS " << _name << " float\n";
      if (!lookupWriten)
      {
        _stream << "LOOKUP_TABLE default\n";
        //  lookupWriten = true;
      }
      for (long j = 0; j < nbNodes; j++)
        _stream << dynelaData->nodes(j)->getNodalValue(field) << "\n";
    }

    // Vector field
    if (fields.getType(field) == 1)
    {
      String _name = fields.getVtklabel(field);
      _stream << "VECTORS " << _name << " float\n";
      for (long j = 0; j < nbNodes; j++)
      {
        Vec3D v = dynelaData->nodes(j)->getNodalVec3D(field);
        _stream << v(0) << " " << v(1) << " " << v(2) << "\n";
      }
    }

    // Tensor field
    if (fields.getType(field) == 2)
    {
      String _name = fields.getVtklabel(field);
      _stream << "TENSORS " << _name << " float\n";
      for (long j = 0; j < nbNodes; j++)
      {
        SymTensor2 t = dynelaData->nodes(j)->getNodalSymTensor(field);
        _stream << t(0, 0) << " " << t(0, 1) << " " << t(0, 2) << " " << t(1, 0) << " " << t(1, 1) << " " << t(1, 2) << " " << t(2, 0) << " " << t(2, 1) << " " << t(2, 2) << "\n";
      }
    }
  }
  _stream << "\n";
} */

/*//-----------------------------------------------------------------------------
void SvgInterface::initFields()
//-----------------------------------------------------------------------------
{
   Field field;
  std::vector<std::string> fieldList;

  // Read the VtkFields line of config file
  dynelaData->settings->getValue("VtkFields", fieldList);

  // Get all data
  for (short i = 0; i < fieldList.size(); i++)
  {
    String st = fieldList.at(i);
    st.strip();
    short nf = field.getField(st);
    if (nf != -1)
      _outputFields << nf;
  }
 }
 */
/* //-----------------------------------------------------------------------------
short SvgInterface::existField(short field)
//-----------------------------------------------------------------------------
{
  for (int i = 0; i < _outputFields.getSize(); i++)
    if (_outputFields(i) == field)
      return i;
  return -1;
}
 */
/* //-----------------------------------------------------------------------------
void SvgInterface::addField(short field)
//-----------------------------------------------------------------------------
{
  if (existField(field) == -1)
    _outputFields << field;
}
 */
/* //-----------------------------------------------------------------------------
void SvgInterface::removeField(short field)
//-----------------------------------------------------------------------------
{
  short ind = existField(field);
  if (ind != -1)
  {
    _outputFields.del(ind);
  }
} */

/* //-----------------------------------------------------------------------------
int SvgInterface::getNumberOfFields()
//-----------------------------------------------------------------------------
{
  return _outputFields.getSize();
} */