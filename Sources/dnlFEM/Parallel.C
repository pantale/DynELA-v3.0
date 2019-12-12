/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file NodeSet.h
  \brief Declaration file for the NodeSet class

  This file is the declaration file for the NodeSet class.

  \ingroup dnlFEM
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <Parallel.h>
#pragma omp default none

/* #include <List.h>
#include <DynELA.h>
#include <Node.h>
#include <Element.h>
#include <Field.h>
 */
//-----------------------------------------------------------------------------
Parallel::Parallel(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;

  // get max number of cores
  _maxThreads = omp_get_max_threads();

  // Init internal lists
  _elementsChunks = _initChunkList();
}

//-----------------------------------------------------------------------------
Parallel::~Parallel()
//-----------------------------------------------------------------------------
{
  _deleteChunkList(_elementsChunks);
}

//-----------------------------------------------------------------------------
ElementsChunk **Parallel::_initChunkList()
//-----------------------------------------------------------------------------
{
  // Init internal lists
  ElementsChunk **_chunk = new ElementsChunk *[_maxThreads];
  for (int chunk = 0; chunk < _maxThreads; chunk++)
  {
    _chunk[chunk] = new ElementsChunk;
  }
  return _chunk;
}

//-----------------------------------------------------------------------------
void Parallel::_deleteChunkList(ElementsChunk **chunkList)
//-----------------------------------------------------------------------------
{
  for (int chunk = 0; chunk < _maxThreads; chunk++)
  {
    delete _elementsChunks[chunk];
  }

  delete _elementsChunks;
}

//-----------------------------------------------------------------------------
void Parallel::setCores(int cores)
//-----------------------------------------------------------------------------
{
  _cores = cores;
  omp_set_num_threads(_cores);
  omp_set_dynamic(false);
}

//-----------------------------------------------------------------------------
void Parallel::dispatchElements(List<Element *> elementList)
//-----------------------------------------------------------------------------
{
  int core = 0;
  for (long elementId = 0; elementId < elementList.getSize(); elementId++)
  {
    Element *pel = elementList(elementId);
    _elementsChunks[core]->elements << pel;
    core++;
    if (core >= _cores)
      core = 0;
  }

  for (core = 0; core < _cores; core++)
    printf("core %d - %ld elements\n", core, _elementsChunks[core]->elements.getSize());
}