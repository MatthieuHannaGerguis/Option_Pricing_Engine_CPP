#include "EuropeanDigitalOption.h"
#include <stdexcept>

EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike)
    : Option(expiry), _strike(strike) {
    if (expiry < 0.0)
        throw std::invalid_argument("Expiry must be positive");
    if (strike <= 0.0)
        throw std::invalid_argument("Strike must be positive for a digital option");
}