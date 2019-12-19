/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

#ifndef __dnlKernel_String_h__
#define __dnlKernel_String_h__

#include <string>

/*!
  \file String.h
  \brief fichier . h de definition des chaines de caracteres
  \ingroup basicTools

  Ce fichier regroupe la definition des chaines de caracteres pour DynELA. Les chaines de caracteres font partie d'une nouvelle classe nommee String qui permet la pluspart des manipulations sur les caracteres. L'ensemble des methodes de cette classe sont documentees.

  Depuis la version v. 1.0.0, cette classe herite de la classe \b std::string de Standard Template Library.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/

/*!
  \class String String.h
  \brief classe de definition des chaines de caracteres
  \ingroup basicTools

  Cette classe sert e l'implementation des chaines de caracteres sur DynELA. Elle s'inspire des exemples donnes par B. Stroutrup dans son livre sur le C++. Des fonctions additionnelles on ete rajoutees par rapport e ce qui est donne dans cet ouvrage.

  Depuis la version v. 1.0.0, cette classe herite de la classe \b std::string de Standard Template Library.

  \author &copy; Olivier PANTALE
  \since DynELA 1.0.0
  \date 1997-2004
*/

class String : public std::string
{

public:
  String();
  String(const char *);
  String(const std::string &);
  String(const String &);
  ~String();

#ifndef SWIG
  String &operator=(const char *);
#endif

  bool contains(char c, int = 0) const;
  bool contains(const String &, int = 0) const;
  bool containsWithCase(const String &, bool) const;
  bool empty() const;
  bool ok() const;
  bool popInt(long &, String = " ");
  bool popReal(double &, String = " ");
  bool popString(String &, String = " ");
  bool scanFileLine(FILE *);
  char firstchar() const;
  char lastchar() const;
  const char *chars() const;
  double getReal() const;
  double getRealAtPos(int, String = " ");
  int containsWithCaseSub(const String &, bool) const;
  int search(char c, int = 0) const;
  int search(const String &, int = 0) const;
  long getInt() const;
  String &convert(double, const char * = "%lf");
  String &convert(int, short leading=0);
  String &convert(long, short leading=0);
  String &replace(const char, const char);
  String &replace(const String &, const String &);
  String afterItem(char c, int = 0) const;
  String afterItem(const String &, int = 0) const;
  String afterItem(int) const;
  String afterLast(const String &) const;
  String atItem(char c, int = 0) const;
  String atItem(const String &, int = 0) const;
  String atItem(int, int) const;
  String beforeItem(char c, int = 0) const;
  String beforeItem(const String &, int = 0) const;
  String beforeItem(int) const;
  String beforeLast(const String &) const;
  String fromItem(char c, int = 0) const;
  String fromItem(const String &, int = 0) const;
  String fromItem(int) const;
  String operator()(int, int) const;
  String subString(int, int) const;
  String throughItem(char c, int = 0) const;
  String throughItem(const String &, int = 0) const;
  String throughItem(int) const;
  void addExtension(String);
  void remove(char, int = 0);
  void remove(const String &, int = 0);
  void remove(int, int);
  void fromFile(FILE *);
  void lowerCase();
  void dump(const String &) const;
  void strip();
  void toFile(FILE *);
  void upperCase();
};

#endif
