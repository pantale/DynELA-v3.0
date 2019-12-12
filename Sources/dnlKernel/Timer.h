/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Timer.h
  \brief Definition of the Timer class.

  This file defines all the Timer class.
  \ingroup dnlKernel
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#ifndef __dnlKernel_Timer_h__
#define __dnlKernel_Timer_h__

#include <iostream>
#include <chrono>
#include <ctime>
#include "String.h"
#include "List.h"

class Timer
{
  std::chrono::time_point<std::chrono::system_clock> _initial, _start, _current, _stop;
  bool _cumulate = true;
  bool _run = false;
  double _totalTime = 0;
  long _calls = 0;
  short _level = 0;
  String _fatherName;
  String _timerName;
  bool _flag = false;

public:
  //  Timer();
  Timer(const char *timerName);
  ~Timer() {}
  bool getCumulate();
  bool running();
  double getCurrent();
  double getDelay();
  double getTotal();
  long getCalls();
  short getLevel();
  String getFather();
  String getName();
  void setCumulate(bool);
  void setLevel(short level);
  // void setName(String);
  void start();
  void stop();
  bool getFlag();
  void setFlag(bool flag);
};

class Timers
{
  List<Timer *> timers;

public:
  Timers(){};
  ~Timers() {}
  double getTotalChilds(const char *father);
  int getNumber() { return timers.getSize(); }
  Timer *timer(const char *timerName);
  Timer *timer(int);
  void add(Timer *);
  void saveReport(const char *filename = "");
  void stop();
  void setFlags(bool flag);
  String conv(double value);
};

#endif
