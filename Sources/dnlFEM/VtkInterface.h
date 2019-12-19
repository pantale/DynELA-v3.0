/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file VtkInterface.h
  \brief Declaration file for the VtkInterface class

  This file is the declaration file for the VtkInterface class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlFEM_VtkInterface_h__
#define __dnlFEM_VtkInterface_h__

#include <String.h>
#include <List.h>
#include <iostream>
#include <fstream>

class VtkInterface
{

private:
  std::ofstream _stream;
  String _fileName;
  List<short> _outputFields;

public:
  String name = "_noname_"; //!< Name of the VTK interface

  // constructeurs
  VtkInterface(char *newName = NULL);
  VtkInterface(const VtkInterface &VtkInterface);
  ~VtkInterface();

  void init(String fileName);
  void write();
  void close();
  void initFields();
  void addField(short field);
  void removeField(short field);
  short existField(short field);
  int getNumberOfFields();

#ifndef SWIG
  void headerWrite();
  void nodesWrite();
  void nodesNumbersWrite();
  void elementsWrite();
  void dataWrite();
#endif
};

#endif
