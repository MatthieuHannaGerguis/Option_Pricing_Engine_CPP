#include "AmericanOption.h"


AmericanOption::AmericanOption(double expiry, double strike)
    : Option(expiry), _strike(strike)
{
    if (_strike < 0.0) { // on verifie bien que le strike n'est pas negatif ainsi que l'expiry
        throw std::invalid_argument(" strike is not positive");
    }
}
