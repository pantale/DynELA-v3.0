/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file MacAddress.C
  \brief Definition of the Mac Address class.

  This file defines the Mac Address class.
  \ingroup dnlKernel
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlKernel_LogFile_h__
#define __dnlKernel_LogFile_h__

#include <String.h>
#include <iostream>
#include <fstream>

class LogFile
{
  String _fileName;
  std::ofstream _stream;
  int _verbosity;
  int _level;

public:
  LogFile();
  ~LogFile();

#ifndef SWIG
  friend LogFile &operator<<(LogFile &logFile, const String &string);
  friend LogFile &operator<<(LogFile &logFile, const int &value);
  friend LogFile &operator<<(LogFile &logFile, const short &value);
  friend LogFile &operator<<(LogFile &logFile, const long &value);
  friend LogFile &operator<<(LogFile &logFile, const double &value);
#endif

  void init(String logFileName, int verbosity = 1);
  void close();
  void downLevel();
  void headerWrite();
  void outputString(String string);
  void separatorWrite(String separator = "");
  void setLevel(int lev);
  void setVerbosity(int verbosity) { _verbosity = verbosity; }
  void upLevel();
};

#endif
