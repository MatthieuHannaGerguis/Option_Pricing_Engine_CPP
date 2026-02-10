#pragma once
#include "Option.h"

class BlackScholesPricer;

// Base class for digital options
class EuropeanDigitalOption : public Option {
public:
    enum class optionType { digital_call, digital_put }; // enum permet d'identifer le type d'option
    //payoff = 1 si S <= K pour le put
    // digital_call : payoff = 1 si S >= K pour le call
    virtual optionType GetOptionType() const = 0;

    virtual ~EuropeanDigitalOption() = default; // tres important dans le cadre du polymorphisme
protected:
    explicit EuropeanDigitalOption(double expiry, double strike);
    double getStrike() const { return _strike; }
private:
    double _strike;
    friend class BlackScholesPricer;
    
};
