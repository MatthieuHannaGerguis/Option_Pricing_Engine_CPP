#include "AsianCallOption.h"

AsianCallOption::AsianCallOption(const std::vector<double>& timeSteps, double strike)
    : AsianOption(timeSteps.back(), strike, timeSteps)
{
    if (strike < 0.0) {
        throw std::invalid_argument("strike must be positive for Asian option");
    }
}
        

double AsianCallOption::payoff(double S) const {
    {
        if (S > getStrike()) {
            return (S - getStrike());
        }
        else {
            return 0.0;
        }
    }
}