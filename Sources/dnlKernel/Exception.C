/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*
	Class Exception implementation
*/

#include "Exception.h"

/*!
  \class Exception Exception.C
  \brief Class 
  \ingroup basicTools
  \version 1.0.0
  \date 1997-2002
  \author Olivier PANTALE


*/
//-----------------------------------------------------------------------------
Exception::Exception() : file(""), line(0), function(""), cond(""), exc("")
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Exception::Exception(const char *f, const int l, const char *func, const char *c, const char *e)
    : file(f), line(l), function(func), cond(c), exc(e)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Exception::SetInfo(const char *f, const int l, const char *func, const char *c, const char *e)
//-----------------------------------------------------------------------------
{
  file = f;
  line = l;
  function = func;
  cond = c;
  exc = e;
}

//-----------------------------------------------------------------------------
void Exception::print()
//-----------------------------------------------------------------------------
{
  std::cerr << "The exception: " << exc << "\noccurred in line <" << line
            << "> of file <" << file
            << "> in method:" << std::endl
            << "    " << function << std::endl
            << "The violated condition was: " << std::endl
            << "    " << cond << std::endl;
}
