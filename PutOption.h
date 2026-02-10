#pragma once
#ifndef PROJECT_PUTOPTION_H
#define PROJECT_PUTOPTION_H

#include "EuropeanVanillaOption.h"

class PutOption : public EuropeanVanillaOption {
public:
    // constructeur (appelle le constructeur de la classe de base)
    explicit PutOption(double expiry, double strike)
        : EuropeanVanillaOption(expiry, strike) {}

    // h(z) = max(K - z, 0)
    double payoff(double z) const override {
        const double K = getStrike();
        if (K > z) {
            return K - z;
        }
        return 0.0;
    }

    optionType GetOptionType() const override {
        return optionType::put;
    }
};

#endif // PROJECT_PUTOPTION_H
#pragma once
