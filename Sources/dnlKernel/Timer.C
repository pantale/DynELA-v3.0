/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Timer.C
  \brief Definition of the Timer class.

  This file defines all the Timer class.
  \ingroup dnlKernel
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <Timer.h>
#include <iostream>
#include <fstream>

//!Constructor of the Timer class
/*!
  This method is the default constructor of the Timer class. All timers are initialized to _current time by default.
*/
//-----------------------------------------------------------------------------
Timer::Timer(const char *timerName)
//-----------------------------------------------------------------------------
{
    String name = timerName;

    if (name.contains(':'))
    {
        _timerName = name.afterItem(":");
        _fatherName = name.beforeItem(":");
    }
    else
    {
        _timerName = timerName;
        _fatherName = "";
    }
    _initial = std::chrono::high_resolution_clock::now();
    _start = _stop = _current = _initial;
}

//-----------------------------------------------------------------------------
void Timer::setCumulate(bool cum)
//-----------------------------------------------------------------------------
{
    _cumulate = cum;
}

//-----------------------------------------------------------------------------
bool Timer::getCumulate()
//-----------------------------------------------------------------------------
{
    return _cumulate;
}

//!Store the _current time as the _start time
/*!
  This method stores the _current time as the _start time.
*/
//-----------------------------------------------------------------------------
void Timer::start()
//-----------------------------------------------------------------------------
{
    _run = true;
    _start = std::chrono::high_resolution_clock::now();
}

//!Store the _current time as the _stop time
/*!
  This method stores the _current time as the _stop time.
*/
//-----------------------------------------------------------------------------
void Timer::stop()
//-----------------------------------------------------------------------------
{
    _stop = std::chrono::high_resolution_clock::now();
    _calls++;
    if (_cumulate)
    {
        _totalTime += getDelay();
    }
    _run = false;
}

//!currentField time
/*!
  This method computes the _current CPU, i.e. the _current time - _initial time.
  \return the delay between _current and _initial times
*/
//-----------------------------------------------------------------------------
double Timer::getCurrent()
//-----------------------------------------------------------------------------
{
    _current = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = _current - _initial;
    return elapsed_seconds.count();
}

//!Delay from _start to _stop times
/*!
  This method computes the delay from the _start time to the _stop time.
  \return the delay between _stop and _start times
*/
//-----------------------------------------------------------------------------
double Timer::getDelay()
//-----------------------------------------------------------------------------
{
    std::chrono::duration<double> elapsed_seconds = _stop - _start;
    return elapsed_seconds.count();
}

//!Cumulative _totalTime for the timer
/*!
  This method computes cumulative time. One has to define setCumulate(true).
  \return the cumulative time
*/
//-----------------------------------------------------------------------------
double Timer::getTotal()
//-----------------------------------------------------------------------------
{
    return _totalTime;
}

//-----------------------------------------------------------------------------
long Timer::getCalls()
//-----------------------------------------------------------------------------
{
    return _calls;
}

//-----------------------------------------------------------------------------
bool Timer::running()
//-----------------------------------------------------------------------------
{
    return _run;
}

//-----------------------------------------------------------------------------
String Timer::getName()
//-----------------------------------------------------------------------------
{
    return _timerName;
}

//-----------------------------------------------------------------------------
short Timer::getLevel()
//-----------------------------------------------------------------------------
{
    return _level;
}

//-----------------------------------------------------------------------------
String Timer::getFather()
//-----------------------------------------------------------------------------
{
    return _fatherName;
}

//-----------------------------------------------------------------------------
bool Timer::getFlag()
//-----------------------------------------------------------------------------
{
    return _flag;
}

//-----------------------------------------------------------------------------
void Timer::setFlag(bool flag)
//-----------------------------------------------------------------------------
{
    _flag = flag;
}

//-----------------------------------------------------------------------------
void Timer::setLevel(short level)
//-----------------------------------------------------------------------------
{
    _level = level;
}

//-----------------------------------------------------------------------------
Timer *Timers::timer(int tim)
//-----------------------------------------------------------------------------
{
    if (tim >= timers.getSize())
        return timers(timers.getSize() - 1);
    return timers(tim);
}

//-----------------------------------------------------------------------------
Timer *Timers::timer(const char *timerName)
//-----------------------------------------------------------------------------
{
    for (short tim = 0; tim < timers.getSize(); tim++)
    {
        if (timers(tim)->getName() == timerName)
            return timers(tim);
    }

    // Doesn't exist, create it and return
    Timer *newTimer = new Timer(timerName);
    add(newTimer);
    return newTimer;
}

//-----------------------------------------------------------------------------
void Timers::add(Timer *tim)
//-----------------------------------------------------------------------------
{
    timers << tim;
}

//-----------------------------------------------------------------------------
void Timers::setFlags(bool flag)
//-----------------------------------------------------------------------------
{
    for (short tim = 0; tim < timers.getSize(); tim++)
    {
        timers(tim)->setFlag(flag);
    }
}

//-----------------------------------------------------------------------------
String Timers::conv(double value)
//-----------------------------------------------------------------------------
{

    // Display in µs
    if (value < 1e-6)
    {
        String str;
        return str.convert(value * 1e6) + " µs";
    }

    // Display in ms
    if (value < 1e-3)
    {
        String str;
        return str.convert(value * 1e3) + " ms";
    }

    // Display in H:M:S
    if (value > 3600)
    {
        String str1, str2, str3;
        int hour, min, sec;
        hour = int(value / 3600);
        min = int((value - 3600 * hour) / 60);
        sec = int(value - 3600 * hour - 60 * min);
        return str1.convert(hour, 2) + ":" + str2.convert(min, 2) + ":" + str3.convert(sec, 2);
    }

    // Display in M:S
    if (value > 60)
    {
        String str1, str2;
        int min, sec;
        min = int(value / 60);
        sec = int(value - 60 * min);
        return str1.convert(min, 2) + ":" + str2.convert(sec, 2);
    }

    // Default display in sec
    String str;
    return str.convert(value) + " s";
}

//-----------------------------------------------------------------------------
void Timers::saveReport(const char *filename)
//-----------------------------------------------------------------------------
{
    double total = 0;
    std::ofstream _stream;

    String fileName = filename;
    if (fileName == "")
    {
        fileName = "CPU-TIMES.log";
    }

    // open the stream
    _stream.open(fileName.chars(), std::fstream::out);

    if (!_stream.is_open())
    {
        fatalError("Timers::saveReport", "Cannot open _stream for file %s", fileName.chars());
    }

    _stream << "# CPU Computing times report\n\n";
    _stream << "Number of tracked processes: " << timers.getSize() << "\n\n";

    // Set all flags to false
    setFlags(false);

    for (short tim = 0; tim < timers.getSize(); tim++)
    {
        Timer *t = timers(tim);
        String father = t->getFather();
        if (!t->getFlag())
        {
            _stream << "\n------------------------------------------\n";
            //  t->setFlag(true);
            if (father != "")
            {
                double fatherTime = timer(t->getFather().chars())->getTotal();
                double cumul = 0;
                _stream << "Group: " << t->getFather() << " (" << conv(fatherTime) << ")\n";
                for (short tim2 = 0; tim2 < timers.getSize(); tim2++)
                {
                    Timer *t2 = timers(tim2);
                    if (!t2->getFlag() && t2->getFather() == t->getFather())
                    {
                        t2->setFlag(true);
                        _stream << "\n"
                                << t2->getName() << "\n";
                        _stream << "cpu-time: " << conv(t2->getTotal()) << " - " << (int((t2->getTotal() / fatherTime) * 10000)) / 100.0 << "%\n";
                        _stream << "calls   : " << t2->getCalls() << "\n";
                        _stream << "cpu-time: " << conv(t2->getTotal() / t2->getCalls()) << " / call\n";
                        cumul += t2->getTotal();
                    }
                }
                _stream << "\nUnmeasured : " << conv(fatherTime - cumul) << " - " << (int(((fatherTime - cumul) / fatherTime) * 10000)) / 100.0 << "%\n";
            }
            else
            {
                t->setFlag(true);
                _stream << t->getName() << "\n";
                _stream << "cpu-time: " << conv(t->getTotal()) << "\n";
                _stream << "calls   : " << t->getCalls() << "\n";
                _stream << "cpu-time: " << conv(t->getTotal() / t->getCalls()) << " / call\n";
            }
        }
    }

    // close the stream
    _stream.close();
}

//-----------------------------------------------------------------------------
void Timers::stop()
//-----------------------------------------------------------------------------
{
    Timer *t;
    for (short tim = 0; tim < timers.getSize(); tim++)
    {
        t = timers(tim);
        if (t->running())
            timers(tim)->stop();
    }
}

//-----------------------------------------------------------------------------
double Timers::getTotalChilds(const char *father)
//-----------------------------------------------------------------------------
{
    int i;
    Timer *t;
    double total = 0;
    bool found = false;
    for (i = 0; i < timers.getSize(); i++)
    {
        t = timers(i);
        if (t->getFather() == father)
        {
            found = true;
            total += getTotalChilds(t->getName().chars());
        }
    }
    if (!found)
    {
        for (i = 0; i < timers.getSize(); i++)
        {
            t = timers(i);
            if (t->getName() == father)
                total += t->getTotal();
        }
    }

    return total;
}