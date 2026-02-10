#ifndef PROJECT_CALLOPTION_H
#define PROJECT_CALLOPTION_H
#include "EuropeanVanillaOption.h"


class CallOption : public EuropeanVanillaOption {
public:
    // Constructor of the EuropeanVanillaOption
    explicit CallOption(double expiry, double strike)
        : EuropeanVanillaOption(expiry, strike) {}

    // h(z) = max(z - K, 0)
    double payoff(double z) const override {
        const double K = getStrike();
        if (z >= K) {
            return z - K;
        }
        return 0.0;
    }

    optionType GetOptionType() const override {
        return optionType::call;
    }
};

#endif //PROJECT_CALLOPTION_H
