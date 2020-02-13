/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file MacAddress.C
  \brief Definition of the Mac Address class.

  This file defines the Mac Address class.
  \ingroup dnlKernel
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <LogFile.h>
#include <System.h>
#include <Errors.h>

//Constructeur de la classe LogFile
/*!
  Le constructeur ouvre le fichier e creer et ajoute automatiquement un entete standard en haut de celui-ci (voir methode headerWrite()).
  \param str nom du fichier log e creer
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
LogFile::LogFile()
//-----------------------------------------------------------------------------
{
  _verbosity = 1;
  _level = 1;
}

//Destructeur de la classe LogFile
/*!
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
LogFile::~LogFile()
//-----------------------------------------------------------------------------
{
  _stream.close();
}

//-----------------------------------------------------------------------------
void LogFile::init(String logFileName, int verbosity)
//-----------------------------------------------------------------------------
{
  if (logFileName == "")
  {
    fatalError("LogFile::LogFile", "Must specify a log filename in the constructor");
  }

  // put the name
  _fileName = logFileName;

  // open the stream
  _stream.open(_fileName.chars(), std::fstream::out);

  if (!_stream.is_open())
  {
    fatalError("LogFile::LogFile", "Cannot open _stream for file %s", _fileName.chars());
  }

  // Header write
  headerWrite();

  _verbosity = verbosity;
  _level = 1;
}

//-----------------------------------------------------------------------------
void LogFile::close()
//-----------------------------------------------------------------------------
{
  // close the stream
  _stream.close();
}

//-----------------------------------------------------------------------------
void LogFile::headerWrite()
//-----------------------------------------------------------------------------
{
  System os;

  // write the header of the file
  // _stream << "DynELA v. " << VERSION << "." << RELEASE << "." << UNDER_RELEASE << "-" << ALPHA_VERSION << std::endl;
  _stream << os.getDate() << std::endl;
  _stream << "Job created by user " << os.getEnvValue("LOGNAME") << std::endl;
  //_stream << "Job executed on a " << os.getCurrHosttype() << " system\n";
  _stream << "Name of the host " << os.getHostname() << std::endl;
  _stream << "Host ID " << os.getHostId() << std::endl;
  _stream << std::endl
          << std::endl;

  separatorWrite();
}

//-----------------------------------------------------------------------------
void LogFile::separatorWrite(String separator)
//-----------------------------------------------------------------------------
{
  _stream << "\n//-----------------------------------------------------------------------------\n";
  _stream << "// " << separator << "\n";
  _stream << "//-----------------------------------------------------------------------------\n\n";

  // force writing information
  _stream.flush();
}

//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const String &string)
//-----------------------------------------------------------------------------
{
  // out the string
  logFile.outputString(string);

  // continue the buffer
  return logFile;
}

//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const long &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const short &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const int &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

//-----------------------------------------------------------------------------
LogFile &operator<<(LogFile &logFile, const double &value)
//-----------------------------------------------------------------------------
{
  String str;
  str.convert(value);

  // out the string
  logFile.outputString(str);

  // continue the buffer
  return logFile;
}

//-----------------------------------------------------------------------------
void LogFile::outputString(String str)
//-----------------------------------------------------------------------------
{
  if (_level <= _verbosity)
  {
    // write the string
    _stream << str.chars();

    // force writing information
    _stream.flush();
  }
}

//reglage de niveau de sortie pour les prochains messages
/*!
  Cette methode permet de specifier que les prochains messages achemines seront de niveau donne en argument. Tous les masseges de niveau plus eleve que celui-ci seront ignores.
  \param lev nouveau niveau
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::setLevel(int lev)
//-----------------------------------------------------------------------------
{
  _level = lev;
}

//reglage de niveau de sortie pour les prochains messages
/*!
  Cette methode permet de specifier que les prochains messages achemines seront de niveau plus eleve d'une unite. Tous les masseges de niveau plus eleve que celui-ci seront ignores.
  \param lev nouveau niveau
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::upLevel()
//-----------------------------------------------------------------------------
{
  _level++;
}

//reglage de niveau de sortie pour les prochains messages
/*!
  Cette methode permet de specifier que les prochains messages achemines seront de niveau plus bas d'une unite. Tous les masseges de niveau plus eleve que celui-ci seront ignores.
  \param lev nouveau niveau
  \author Olivier PANTALE
  \since DynELA 1.0.0
*/
//-----------------------------------------------------------------------------
void LogFile::downLevel()
//-----------------------------------------------------------------------------
{
  _level--;
  if (_level <= 1)
    _level = 1;
}
