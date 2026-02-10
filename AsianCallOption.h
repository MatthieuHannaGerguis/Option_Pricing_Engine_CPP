#pragma once
#include "AsianOption.h"

class AsianCallOption : public AsianOption {
public:
    AsianCallOption(const std::vector<double>& timeSteps, double strike);

    double payoff(double S) const override;
};
