#pragma once
#include "Option.h"
#include <stdexcept>

// représente une option américaine exerçable à tout moment avant l'échéance.
class AmericanOption : public Option {
protected:
    double _strike; // prix d’exercice
    double getStrike() const;

public:
    explicit AmericanOption(double expiry, double strike);

    // identification
    bool isAmericanOption() const override { return true; }


    virtual double payoff(double S) const = 0;

    virtual ~AmericanOption() = default; // destructeur au cas ou

};