/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file System.h
  \brief Definition of the System class and OS operators.

  This file defines all the OS operators in the System class.
  \ingroup dnlKernel
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlKernel_System_h__
#define __dnlKernel_System_h__

#include <String.h>

/*!
  \brief System class definition.
  \ingroup dnlKernel
  \date 1997-2019
  \author &copy; Olivier PANTALE
  
This class contains a set of methods dedicated to the System working environment. 
These methods are used to retrieve system environment variables from a program, the current date, time and some definitions of directories and program paths, all in a System environment. 
This class also contains methods used to launch external applications.
\warning This class is related to the Unix system. If someone wants to port this program to a Windows environment, this has to be changed.
*/
class System
{

public:
  System() {}
  ~System() {}
  int execute(const String &cmd);
  String getEnvValue(const String &envname);
  bool existEnvValue(const String &envname);
  String getLogin();
  String getHostname();
  String getDate(bool full = true);
  String getTime();
  long getHostId();
  String getPathName();
  String getExePath();
};

/* int compareIndice(const void *p1, const void *p2);
 */
bool argumentsLineParse(char *rr, int argc, char **argv);
long argumentsLinePosition(char *rr, int argc, char **argv);
char *argumentsLineGet(char *rr, int argc, char **argv);

#endif
