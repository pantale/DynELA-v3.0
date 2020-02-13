/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file System.C
  \brief Fichier .C de definitions associees e l'OS System
  \ingroup basicTools

  Ce fichier regroupe la definition de la classe System utilisee pour l'environnement de travail System.

  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
  \date 1997-2004
*/

#include <System.h>
#include <Errors.h>
#include <Exception.h>

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>

//execution d'une commande systeme
/*!
  Cette methode execute une commande externe. Lancement d'un nouveau processus par l'intermediaire de la commande systeme \b system(). Le nouveau programme est un nouveau processus totalement independant de l'application actuelle. Le seul lien est que le programme appelant attend la fin de l'execution du nouveau processus pour continuer les operations (saus si on utilise la parametre magique & dans la ligne de commande... Vous connaissez System ?). Comme cette commande est simple, l'environnement de travail pour l'execution du nouveau process est /bin/sh. Un message d'erreur est genere si un probleme e ete rencontre au cours de l'execution.

  Pour des situations plus complexes, il faudra faire appel e des methodes de lancement de programme par les "classiques" fork() et exec(). Mais le ea conduit inevitablement aux problemes de \b threads et c'est une autre histoire en fait. ;-0

  \param cmd chaine de caracteres de type \b String definissant la commande systeme e lancer
  \return valeur indiquant si l'execution de la commande s'est executee sans problemes. Une valeur 0 est retournee si tout e bien fonctionne, une autre valeur est retournee si un probleme e ete rencontre au cours de l'execution.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
int System::execute(const String &cmd)
//-----------------------------------------------------------------------------
{
  int status = system(cmd.chars());

  if (status != 0)
  {
    if (status == 127)
    {
      std::cerr << "/bin/sh could not be executed\n";
    }
    std::cerr << "System::execute()\n";
    std::cerr << "Last System command execution:\n";
    std::cerr << cmd.chars() << "\nfailed (return value=" << status << ")\n";
  }

  return status;
}

//recupere la valeur associee e une variable d'environnement
/*!
  Cette methode recupere la valeur associee e une variable d'environnement System. Si cette variable n'est pas definie, cette methode renvoie la chaine de caracteres suivante "cannot get \<var\> value". La valeur retournee est de type String.
\param envname nom de la variable d'environnement
\return valeur associee e la variable d'environnement sous forme de String
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String System::getEnvValue(const String &envname)
//-----------------------------------------------------------------------------
{
  String s;
  char *c = std::getenv(envname.chars());

  // if variable is defined
  if (c != NULL)
  {
    s = c;
    s.strip();
  }

  // else if not defined
  else
    s = "cannot get " + envname + " value";

  // return the string
  return (s);
}

//teste la presence d'une variable d'environnement definie
/*!
  Cette methode teste la definition d'une variable d'environnement. Elle renvoie une valeur booleenne qui renseigne sur l'etat de definition de cette variable d'environnement.
\param envname nom de la variable d'environnement
\return \c true si la variable d'environnement est definie sur le systeme, \c false dans le cas contraire.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
bool System::existEnvValue(const String &envname)
//-----------------------------------------------------------------------------
{
  char *c = std::getenv(envname.chars());

  // test if defined
  if (c != NULL)
    return true;

  // not defined
  return false;
}

//renvoie le login de l'utilisateur
/*!
  Cette methode renvoie le login de l'utilisateur du systeme sous forme d'une chaine de caracteres.
\return login de l'utilisateur ou "unknown user" si cette information ne peut pas etre donnee
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String System::getLogin()
//-----------------------------------------------------------------------------
{
  struct passwd *pwd;
  try
  {
    pwd = getpwuid(getuid());
    IfException(pwd == NULL, StandardExceptions::NullPointer);
  }
  catch (Exception &e)
  {
    e.print();
    return "";
  }

  return (pwd->pw_name);
}

//renvoie le nom de la machine
/*!
  Cette methode renvoie le nom de la machine sur laquelle l'application est en cours d'execution.
\return nom de la machine ou "unknown host" si cette information ne peut pas etre donnee
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String System::getHostname()
//-----------------------------------------------------------------------------
{
  String s;
  char hostname[80];

  try
  {
    int ret = gethostname(hostname, 80);
    IfException(ret != 0, StandardExceptions::NullPointer);
    s = hostname;
    s.strip();
  }
  catch (Exception &e)
  {
    s = "";
  }

  return (s);
}

//renvoie la date et l'heure courante
/*!
  Cette methode renvoie la date et l'heure courante au niveau du systeme.
\param full : cette valeur booleenne definit la nature du resultat retourne. Si la valeur est true le format est alors le format comple sous la forme (Fri Jan 25 15:08:24 2002) si cette valeur est false, alors le format retourne est le format court de la forme (Jan 25, 2002). La valeur par defaut si rien n'est precise est true.
\return date et heure actuelle ou "unknown date" si cette information ne peut pas etre donnee
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String System::getDate(bool full)
//-----------------------------------------------------------------------------
{
  char *thedate;

  time_t theclock;

  theclock = time(0);
  thedate = (char *)ctime(&theclock);

  String s;
  if (thedate != NULL)
  {
    // Full format: Fri Aug 20 14:23:26 1993
    s = thedate;
    s.strip();

    // Short format: Aug 20, 1993
    if (!full)
    {
      s.remove(0, 4); // Kill day + space, e.g. Fri
      int i = 5;
      while (s[i] != ' ')
        ++i;
      s.remove(i + 1, 8); // Kill time and year and spaces, e.g. 13:00:00 1993
      s[6] = ',';
    }
  }

  else
    s = "unknown date";

  return (s);
}

//renvoie l'heure courante
/*!
  Cette methode renvoie l'heure courante au niveau du systeme.
  \return valeur de l'heure courante sous forme d'un String (format : 14:23:26)
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String System::getTime()
//-----------------------------------------------------------------------------
{
  String s = getDate(true);

  // Time format is 14:23:26
  s.remove(0, 11);
  s = s.beforeItem(8);

  return (s);
}

//renvoie le nom du repertoire courant
/*!
  Cette methode renvoie le nom du repertoire courant depuis lequel l'execution e ete lancee.
  \return repertoire courant ou "unknown pathname" si cette information ne peut etre donnee.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
String System::getPathName()
//-----------------------------------------------------------------------------
{
  char pathname[2500];
  if (getcwd(pathname, 2500) == NULL)
  {
    fatalError("System::getPathName()",
               "getPathName()\nsomething went wrong when calling the system function getcwd\n");
  }

  String s;
  if (strlen(pathname) > 0)
  {
    s = pathname;
    s.strip();
  }

  else
    s = "unknown pathname";

  return (s);
}

//-----------------------------------------------------------------------------
String System::getExePath()
//-----------------------------------------------------------------------------
{
  int bufsize = 1024;
  char buf[bufsize];
  ssize_t count = readlink("/proc/self/exe", buf, bufsize);
  const char *path;
  if (count != -1)
  {
    path = dirname(buf);
  }
  return path;
}

//renvoie le hostid de la machine
/*!
  Cette methode renvoie le hostid de la machine sur laquelle le programme est execute. Le hostid est une valeur entiere generalement donnee sous une forme hexadecimale 0xFFFFFFFF sur 32 bits. Ce numero est unique par machine.
  \return hostId de la machine.
  \author &copy; Olivier PANTALE
  \since DynELA 0.9.5
*/
//-----------------------------------------------------------------------------
long System::getHostId()
//-----------------------------------------------------------------------------
{
  return gethostid();
}

/* //-----------------------------------------------------------------------------
int compareIndice(const void *p1, const void *p2)
//-----------------------------------------------------------------------------
{
  return *((long *)p1) < *((long *)p2);
}
 */

//-----------------------------------------------------------------------------
bool argumentsLineParse(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (long i = 1; i < argc; i++)
  {

    // test d'argument
    if (strncmp(rr, argv[i] + 1, strlen(rr)) == 0)
    {
      // cout << argv[i]+1 << " ";

      // retour
      return true;
    }
  }

  // retour
  return false;
}

//-----------------------------------------------------------------------------
long argumentsLinePosition(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (long i = 1; i < argc; i++)
  {

    // test d'argument
    if (strncmp(rr, argv[i] + 1, strlen(rr)) == 0)
    {
      // cout << argv[i]+1 << " ";

      // retour
      return i;
    }
  }

  // retour
  return 0;
}

//-----------------------------------------------------------------------------
char *argumentsLineGet(char *rr, int argc, char **argv)
//-----------------------------------------------------------------------------
{
  // boucle de parsing des arguments
  for (long i = 1; i < argc; i++)
  {

    // test d'argument
    if (strcmp(rr, argv[i] + 1) == 0)
    {
      // cout << argv[i]+1 << " ";

      // retour
      return argv[i + 1];
    }
  }

  // retour
  return NULL;
}
