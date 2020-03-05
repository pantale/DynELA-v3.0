/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Timer.h
  \brief Definition of the Timer class.

  This file defines all the Timer class.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_Timer_h__
#define __dnlKernel_Timer_h__

#include <iostream>
#include <chrono>
#include <ctime>
#include <String.h>
#include <List.h>

class Timer
{
  bool _cumulate = true;
  bool _flag = false;
  bool _run = false;
  double _totalTime = 0;
  long _calls = 0;
  short _level = 0;
  std::chrono::time_point<std::chrono::system_clock> _initial, _start, _current, _stop;
  String _fatherName;
  String _timerName;

public:
  Timer(const char *timerName);
  ~Timer() {}

  bool getCumulate();
  bool getFlag();
  bool running();
  double getCurrent();
  double getDelay();
  double getTotal();
  long getCalls();
  short getLevel();
  String getFather();
  String getName();
  void setCumulate(bool);
  void setFlag(bool flag);
  void setLevel(short level);
  void start();
  void stop();
};

class Timers
{
  List<Timer *> timers;

public:
  Timers(){};
  ~Timers() {}

  double getTotalChilds(const char *father);
  int getNumber() { return timers.getSize(); }
  String conv(double value);
  Timer *timer(const char *timerName);
  Timer *timer(int);
  void add(Timer *);
  void saveReport(const char *filename = "");
  void setFlags(bool flag);
  void stop();
};

#endif
