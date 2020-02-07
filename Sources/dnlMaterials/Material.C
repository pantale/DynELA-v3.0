/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  By Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2019                                                *
 *                                                                         *
 **************************************************************************/

/*!
  \file Material.C
  \brief Definition file for the Material class

  This file is the definition file for the Material class.

  \ingroup dnlMaterials
  \author &copy; Olivier PANTALE
  \date 1997-2019
*/

#include <Material.h>
#include <HardeningLaw.h>

//-----------------------------------------------------------------------------
Material::Material(char *newName)
//-----------------------------------------------------------------------------
{
  if (newName != NULL)
    name = newName;
}

//-----------------------------------------------------------------------------
Material::Material(const Material &X)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Material::~Material()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
bool Material::checkValues()
//-----------------------------------------------------------------------------
{
  // check poisson value
  if ((poissonRatio < 0.) || (poissonRatio > 0.5))
    fatalError("Materials integrity check",
               "material %s have a poisson value of %lf\n", name.chars(),
               poissonRatio);

  // check Young modulus
  if (youngModulus <= 0.)
    fatalError("Materials integrity check",
               "material %s have a young value of %lf\n", name.chars(), youngModulus);

  // check density
  if (density <= 0.)
    fatalError("Materials integrity check",
               "material %s have a density value of %lf\n", name.chars(),
               density);

  return true;
}

//-----------------------------------------------------------------------------
void Material::computeHookeTensor()
//-----------------------------------------------------------------------------
{
  long i, j, k, l;
  double mu, lambda;

  lambda = getLambda();   // poissonRatio * youngModulus / ((1. - 2. * poissonRatio) * (1. + poissonRatio));
  mu = getShearModulus(); // youngModulus / (2. * (1. + poissonRatio));

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
        for (l = 0; l < 3; l++)
        {
          _hookeTensor(i, j, k, l) = lambda * (dnlKronecker(i, j) * dnlKronecker(k, l)) + mu * (dnlKronecker(i, k) * dnlKronecker(j, l) + dnlKronecker(i, l) * dnlKronecker(j, k));
        }
}

//-----------------------------------------------------------------------------
void Material::setColor(double red, double green, double blue)
//-----------------------------------------------------------------------------
{
  _color(0) = red;
  _color(1) = green;
  _color(2) = blue;
}

//-----------------------------------------------------------------------------
Matrix Material::getHookeMatrix(short type)
//-----------------------------------------------------------------------------
{
  Matrix hookeMatrix;

  switch (type)
  {
  case planeStrain:
    hookeMatrix.redim(3, 3);
    hookeMatrix = 0.0;
    hookeMatrix(0, 0) = 1.0;
    hookeMatrix(1, 1) = 1.0;
    hookeMatrix(2, 2) = (1.0 - 2.0 * poissonRatio) / (2.0 * (1.0 - poissonRatio));
    hookeMatrix(1, 0) = hookeMatrix(0, 1) = poissonRatio / (1.0 - poissonRatio);
    hookeMatrix *= (youngModulus * (1.0 - poissonRatio) / ((1.0 + poissonRatio) * (1.0 - 2.0 * poissonRatio)));
    break;

  case planeStress:
    hookeMatrix.redim(3, 3);
    hookeMatrix = 0.0;
    hookeMatrix(0, 0) = 1.0;
    hookeMatrix(1, 1) = 1.0;
    hookeMatrix(2, 2) = (1.0 - poissonRatio) / 2;
    hookeMatrix(1, 0) = hookeMatrix(0, 1) = poissonRatio;
    hookeMatrix *= (youngModulus / (1.0 - dnlSquare(poissonRatio)));
    break;

  case axisymetric:
    hookeMatrix.redim(4, 4);
    hookeMatrix = 0.0;
    hookeMatrix(0, 0) = 1.0;
    hookeMatrix(1, 1) = 1.0;
    hookeMatrix(2, 2) = (1.0 - 2.0 * poissonRatio) / (2. * (1.0 - poissonRatio));
    hookeMatrix(3, 3) = 1.0;
    hookeMatrix(1, 0) = hookeMatrix(0, 1) = poissonRatio / (1.0 - poissonRatio);
    hookeMatrix(3, 0) = hookeMatrix(0, 3) = poissonRatio / (1.0 - poissonRatio);
    hookeMatrix(3, 1) = hookeMatrix(1, 3) = poissonRatio / (1.0 - poissonRatio);
    hookeMatrix *= (youngModulus * (1.0 - poissonRatio) / ((1.0 + poissonRatio) * (1.0 - 2.0 * poissonRatio)));
    break;

  case threedimensional:
    hookeMatrix.redim(6, 6);
    hookeMatrix = 0.0;
    hookeMatrix(0, 0) = 1.0;
    hookeMatrix(1, 1) = 1.0;
    hookeMatrix(2, 2) = 1.0;
    hookeMatrix(3, 3) = (1.0 - 2.0 * poissonRatio) / (2. * (1.0 - poissonRatio));
    hookeMatrix(4, 4) = (1.0 - 2.0 * poissonRatio) / (2. * (1.0 - poissonRatio));
    hookeMatrix(5, 5) = (1.0 - 2.0 * poissonRatio) / (2. * (1.0 - poissonRatio));
    hookeMatrix(0, 1) = hookeMatrix(1, 0) = poissonRatio / (1.0 - poissonRatio);
    hookeMatrix(0, 2) = hookeMatrix(2, 0) = poissonRatio / (1.0 - poissonRatio);
    hookeMatrix(1, 2) = hookeMatrix(2, 1) = poissonRatio / (1.0 - poissonRatio);
    hookeMatrix *= (youngModulus * (1.0 - poissonRatio) / ((1.0 + poissonRatio) * (1.0 - 2.0 * poissonRatio)));
    break;

  default:
    fatalError("Material::getHookeMatrix", "Matrix not computed for this case");
  }
  return hookeMatrix;
}

//-----------------------------------------------------------------------------
void Material::setHardeningLaw(HardeningLaw *hardeningLaw)
//-----------------------------------------------------------------------------
{
  _hardening = hardeningLaw;
  hardeningLaw->add(this);
}

//-----------------------------------------------------------------------------
HardeningLaw *Material::getHardeningLaw() const
//-----------------------------------------------------------------------------
{
  return _hardening;
}

//-----------------------------------------------------------------------------
bool Material::isYield()
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->isYield());
  return false;
}

/*
//-----------------------------------------------------------------------------
void Material::print(ostream &os) const
//-----------------------------------------------------------------------------
{
  os << "material: " << name << endl;
  os << "youngModulus= " << youngModulus << endl;
  os << "nu=" << poissonRatio << endl;
  os << "density=" << density << endl;
  os << "heatCapacity=" << heatCapacity << endl;
  os << "initialTemperature=" << initialTemperature << endl;

  // complement d'ecriture de la partie ecrouissage isotrope
  if (_hardening != NULL)
    _hardening->print(os);
}

//-----------------------------------------------------------------------------
ostream &operator<<(ostream &os, Material &mat)
//-----------------------------------------------------------------------------
{
  mat.print(os);
  return os;
}

//-----------------------------------------------------------------------------
void Material::toFile(FILE *pfile)
//-----------------------------------------------------------------------------
{
  // fprintf(pfile,"MATERIAL %4d\n",);
  fprintf(pfile, "MATERIAL %s\n", name.chars());
  fprintf(pfile, "    YOUNG'S      POISSON'S    DENSITY\n");
  fprintf(pfile, "    MODULUS        RATIO\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E\n\n", youngModulus, poissonRatio, density);
  fprintf(pfile, "    INITIAL       HEAT      DILATATION     THERMAL\n");
  fprintf(pfile, "  TEMPERATURE   CAPACITY       COEFF     CONDUCTIVITY\n");
  fprintf(pfile, "  %8.5E  %8.5E  %8.5E  %8.5E\n\n", initialTemperature, heatCapacity, dilatation, thermalConductivity);

  // complement d'ecriture de la partie ecrouissage isotrope
  if (_hardening != NULL)
    _hardening->toFile(pfile);
}

//-----------------------------------------------------------------------------
String Material::convertToDynELASourceFile(String name)
//-----------------------------------------------------------------------------
{
  String str;
  char sstr[1000];
  str = "";

  sprintf(sstr, "%s.setHeatCoefficient(%12.7E);\n", name.chars(), heatCoeff());
  str += sstr;
  sprintf(sstr, "%s.setDilatation(%12.7E);\n", name.chars(), dilat());
  str += sstr;
  sprintf(sstr, "%s.setInitTemperature(%12.7E);\n", name.chars(), initTemp());
  str += sstr;
  sprintf(sstr, "%s.setConductivity(%12.7E);\n", name.chars(), conductivity());
  str += sstr;
  sprintf(sstr, "%s.setYoung(%12.7E);\n", name.chars(), young());
  str += sstr;
  sprintf(sstr, "%s.setPoisson(%12.7E);\n", name.chars(), nu());
  str += sstr;
  sprintf(sstr, "%s.setDensity(%12.7E);\n", name.chars(), density());
  str += sstr;
  sprintf(sstr, "%s.setColor(%12.7E, %12.7E, %12.7E);\n", name.chars(), _color(0), _color(1), _color(2));
  str += sstr;

  // attach the isotropic hardening part to the material if exist
  if (_hardening != NULL)
  {
    sprintf(sstr, "%s.setHardening(%s);\n", name.chars(), _hardening->name.chars());
    str += sstr;
  }

  return str;
}



//-----------------------------------------------------------------------------
double Material::getYieldStress(IntegrationPoint *pfield)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getYieldStress(pfield));
  return 0;
}

//-----------------------------------------------------------------------------
double Material::getDerYieldStress(IntegrationPoint *pfield)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getDerYieldStress(pfield));
  return 0;
}

//-----------------------------------------------------------------------------
double Material::getIsotropicYieldStress(Element *el, double shift)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getIsotropicYieldStress(el, shift));
  return 0;
}

//-----------------------------------------------------------------------------
double Material::getIsotropicYieldHardening(Element *el, double shift)
//-----------------------------------------------------------------------------
{
  if (_hardening != NULL)
    return (_hardening->getIsotropicYieldHardening(el, shift));
  return 0;
}



*/