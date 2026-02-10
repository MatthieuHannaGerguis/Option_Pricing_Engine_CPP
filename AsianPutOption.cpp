#include "AsianPutOption.h"

AsianPutOption::AsianPutOption(const std::vector<double>& timeSteps, double strike)
    : AsianOption(timeSteps.back(), strike, timeSteps)
{}

double AsianPutOption::payoff(double S) const {
    return (S < getStrike()) ? (getStrike() - S) : 0.0;
}