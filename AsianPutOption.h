#pragma once
#include "AsianOption.h"

class AsianPutOption : public AsianOption {
public:
    AsianPutOption(const std::vector<double>& timeSteps, double strike);
    double payoff(double S) const override;
};