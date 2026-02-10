//Ilian Segoin on 23/10/2025.

#include "EuropeanVanillaOption.h"
#include <stdexcept>  //  nécessaire pour std::invalid_argument

/*
  - Appelle le constructeur de base Option(expiry)
  - Vérifie expiry >= 0 et strike >= 0
*/
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike)
    : Option(expiry), _strike(strike)
{
    if (expiry < 0.0) // vérifie que les valeurs ne sont pas négatives
        throw std::invalid_argument("expiry must be nonnegative");
    if (strike < 0.0)
        throw std::invalid_argument("strike must be nonnegative");
}