/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file MacAddress.C
  \brief Definition of the Mac Address class.

  This file defines the Mac Address class.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_MacAddress_h__
#define __dnlKernel_MacAddress_h__

#include <String.h>
#include <List.h>

/*!
  \brief MacAddress class definition.
  \ingroup dnlKernel
  
This class contains a set of methods dedicated to the MacAddress working environment. 
*/
class MacAddress
{
  unsigned char mac[6]; //!< Mac address stored as 6 chars
  String interface;     //!< Name of the interface

  friend class MacAddresses;

public:
  MacAddress() {}  //!< default constructor of the MacAddress class
  ~MacAddress() {} //!< default destructor of the MacAddress class
};

/*!
  \brief MacAddresses class definition.
  \ingroup dnlKernel
  
This class contains a set of methods dedicated to the MacAddresses working environment. The MacAddresses class is used to manage the different Mac Address of a computer.
*/
class MacAddresses
{
  List<MacAddress *> macs; //!< List of mac adresses

public:
  MacAddresses();
  ~MacAddresses() {} //!< default destructor of the MacAddresses class
  int getNumber();
  String getAddress(int);
  String getInterface(int);
};

#endif
