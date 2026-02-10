#pragma once
#ifndef PROJECT_EUROPEANDIGITALPUTOPTION_H
#define PROJECT_EUROPEANDIGITALPUTOPTION_H

#include "EuropeanDigitalOption.h"

class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:
    explicit EuropeanDigitalPutOption(double expiry, double strike);

    double payoff(double S) const override;
    optionType GetOptionType() const override;


};

#endif // PROJECT_EUROPEANDIGITALPUTOPTION_H
