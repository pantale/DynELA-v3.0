/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file CrackFunction.C
  \brief Definition file for the crack function class

  This file is the definition file for the crack function class. A crack function is a function with the following form: \f[ y=f(x) \f] defined from a set of points.
  Linear interpolation is used to obtain data between given points.
  \ingroup dnlMaths
*/

#include <Macros.h>
#include <CrackFunction.h>

//Constructor of the CrackFunction class
//-----------------------------------------------------------------------------
CrackFunction::CrackFunction()
//-----------------------------------------------------------------------------
{
    style = 0;
    style2 = 0;
    Sort = false;
}

//Copy constructor of the CrackFunction class
//-----------------------------------------------------------------------------
CrackFunction::CrackFunction(const CrackFunction &X)
//-----------------------------------------------------------------------------
{
}

//Destructor of the CrackFunction class
//-----------------------------------------------------------------------------
CrackFunction::~CrackFunction()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void CrackFunction::headTipAdd(double x, double y)
//-----------------------------------------------------------------------------
{
    DiscreteFunction::add(x, y);
}

//-----------------------------------------------------------------------------
void CrackFunction::tailTipAdd(double x, double y)
//-----------------------------------------------------------------------------
{
    DiscreteFunction::startPointAdd(x, y);
}

//-----------------------------------------------------------------------------
bool CrackFunction::distanceToSegmentShort(double &dist, long i, double x, double y)
//-----------------------------------------------------------------------------
{
    dist = -1;
    double x1, x2, y1, y2;
    x1 = lpoints(i)->x;
    x2 = lpoints(i + 1)->x;
    y1 = lpoints(i)->y;
    y2 = lpoints(i + 1)->y;

    // cas particulier // x
    if (y1 == y2)
    {
        if (x < dnlMin(x1, x2))
        {
            return false;
        }
        if (x > dnlMax(x1, x2))
        {
            return false;
        }
        dist = (y - y1);
        return true;
    }

    // cas particulier // y
    if (x1 == x2)
    {
        if (y < dnlMin(y1, y2))
        {
            return false;
        }
        if (y > dnlMax(y1, y2))
        {
            return false;
        }
        dist = (x - x1);
        return true;
    }

    if ((x2 - x1) * (x - x1) + (y2 - y1) * (y - y1) < 0)
    {
        return false;
    }
    if ((x1 - x2) * (x - x2) + (y1 - y2) * (y - y2) < 0)
    {
        return false;
    }

    // cas general ax-y+b=0
    double a, c;
    a = (y2 - y1) / (x2 - x1);
    c = y1 - a * x1;
    dist = (a * x - y + c) / sqrt(dnlSquare(a) + 1);
    return true;
}

//-----------------------------------------------------------------------------
bool CrackFunction::isOnPositiveSide(double x, double y)
//-----------------------------------------------------------------------------
{
    long nearSeg, nearPoint;
    double distMinSeg, distMinPoint, dist;
    long i;
    double x1, y1, x2, y2, vx, vy;

    // moins de 2 points ?
    if (lpoints.getSize() < 2)
    {
        return false;
    }

    // point le plus proche
    nearSeg = -1;
    distMinSeg = 1e30;
    nearPoint = -1;
    distMinPoint = 1e30;

    // distance mini segment
    for (i = 0; i < lpoints.getSize() - 1; i++)
    {
        if (distanceToSegmentShort(dist, i, x, y) == true)
        {
            if (dnlAbs(dist) < distMinSeg)
            {
                distMinSeg = dnlAbs(dist);
                nearSeg = i;
            }
        }
    }

    // distance mini point
    for (i = 0; i < lpoints.getSize(); i++)
    {
        dist = dnlSquare(lpoints(i)->x - x) + dnlSquare(lpoints(i)->y - y);
        if (dist == 0.)
        {
            nearPoint = i;
            distMinPoint = dist;
            i = lpoints.getSize();
        }
        if (dist < distMinPoint)
        {
            distMinPoint = dist;
            nearPoint = i;
        }
    }
    distMinPoint = sqrt(distMinPoint);

    // traitement distance mini-segment
    if ((nearSeg >= 0) && (distMinSeg <= distMinPoint))
    {
        x1 = lpoints(nearSeg)->x;
        x2 = lpoints(nearSeg + 1)->x;
        y1 = lpoints(nearSeg)->y;
        y2 = lpoints(nearSeg + 1)->y;
        return ((y1 - y2) * (x - x1) + (x2 - x1) * (y - y1) >= 0);
    }

    //   cout << nearSeg << endl;

    // near the tip
    if (nearPoint == 0)
    {
        x2 = x - lpoints(nearPoint)->x;
        y2 = y - lpoints(nearPoint)->y;
        x1 = -(lpoints(nearPoint + 1)->y - lpoints(nearPoint)->y);
        y1 = lpoints(nearPoint + 1)->x - lpoints(nearPoint)->x;
        return (x2 * x1 + y2 * y1 >= 0);
    }

    // nearPoint the other tip
    if (nearPoint == lpoints.getSize() - 1)
    {
        x2 = x - lpoints(nearPoint)->x;
        y2 = y - lpoints(nearPoint)->y;
        x1 = -(lpoints(nearPoint)->y - lpoints(nearPoint - 1)->y);
        y1 = lpoints(nearPoint)->x - lpoints(nearPoint - 1)->x;
        return (x2 * x1 + y2 * y1 >= 0);
    }

    // vecteurs normaux au point
    vx = x - lpoints(nearPoint)->x;
    vy = y - lpoints(nearPoint)->y;
    x1 = -(lpoints(nearPoint)->y - lpoints(nearPoint - 1)->y);
    y1 = lpoints(nearPoint)->x - lpoints(nearPoint - 1)->x;
    x2 = -(lpoints(nearPoint + 1)->y - lpoints(nearPoint)->y);
    y2 = lpoints(nearPoint + 1)->x - lpoints(nearPoint)->x;
    double cote = x1 * y2 - y1 * x2;
    double p1 = x1 * vx + y1 * vy;
    double p2 = x2 * vx + y2 * vy;

    if (cote >= 0)
    {
        if ((p1 >= 0) && (p2 >= 0))
        {
            return true;
        }
        return false;
    }
    else
    {
        if ((p1 <= 0) && (p2 <= 0))
        {
            return false;
        }
        return true;
    }

    return false;
}
