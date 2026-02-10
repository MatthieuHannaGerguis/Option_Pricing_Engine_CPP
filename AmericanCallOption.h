#pragma once
#include "AmericanOption.h"

// option Américaine de type Call 
class AmericanCallOption : public AmericanOption {
public:
    explicit AmericanCallOption(double expiry, double strike);
    double payoff(double S) const override; // notre payoff est  max(S - K, 0)

};
