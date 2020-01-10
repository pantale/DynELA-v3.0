/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

%varargs(30, long val = 0) DynELA::createElement;

%{
  #include "Model.h"
  #include "DynELA.h"
  #include "VtkInterface.h"
  #include "SvgInterface.h"
  #include "Drawing.h"
  #include "HistoryFile.h"
  #include "Solver.h"
  #include "ExplicitOld.h"
  #include "Explicit.h"
  #include "Parallel.h"
%}

%include "Model.h"
%include "DynELA.h"
%include "VtkInterface.h"
%include "SvgInterface.h"
%include "Drawing.h"
%include "HistoryFile.h"
%include "Solver.h"
%include "ExplicitOld.h"
%include "Explicit.h"
%include "Parallel.h"