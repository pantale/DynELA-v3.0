/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

%{
  #include "ElasticLaw.h"
  #include "ElastoplasticLaw.h"
  #include "HardeningLaw.h"
  #include "IntegrationPoint.h"
  #include "JohnsonCookLaw.h"
  #include "Material.h"
  #include "TabularLaw.h"
%}

%include "IntegrationPoint.h"

%include "Material.h"
%include "HardeningLaw.h"

%include "ElasticLaw.h"
%include "ElastoplasticLaw.h"
%include "JohnsonCookLaw.h"
%include "TabularLaw.h"