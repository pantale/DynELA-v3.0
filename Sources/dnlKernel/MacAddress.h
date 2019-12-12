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

#ifndef __dnlKernel_MacAddress_h__
#define __dnlKernel_MacAddress_h__

#include "String.h"
#include "List.h"

/*!
  \brief MacAddress class definition.
  \ingroup dnlKernel
  \date 1997-2019
  \author &copy; Olivier PANTALE
  
This class contains a set of methods dedicated to the MacAddress working environment. 
*/
class MacAddress
{
  unsigned char mac[6];
  String interface;

  friend class MacAddresses;

public:
  MacAddress() {}
  ~MacAddress() {}
};

/*!
  \brief MacAddress class definition.
  \ingroup dnlKernel
  \date 1997-2019
  \author &copy; Olivier PANTALE
  
This class contains a set of methods dedicated to the MacAddresses working environment. The MacAddresses class is used to manage the different Mac Address of a computer.
*/
class MacAddresses
{
  List<MacAddress *> macs;

public:
  MacAddresses();
  ~MacAddresses() {}
  int getNumber() { return macs.getSize(); }
  String getAddress(int);
  String getInterface(int);
};

#endif
