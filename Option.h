#pragma once

#ifndef PROJECT_OPTION_H
#define PROJECT_OPTION_H
#include <vector>


class Option {
private:
    double _expiry;

public:
    // Constructeur (initialise _expiry)
    explicit Option(double expiry) : _expiry(expiry) {}
    virtual ~Option() = default;     // Destructeur virtuel protégé (bonne pratique)


    double getExpiry() const;

    // Payoff function h(S): pure virtual (classe abstraite)
    virtual double payoff(double S) const = 0; // pour single price payoff

    virtual double payoffPath(const std::vector<double>& path) const {
        if (path.empty()) { //pour des paths dependents options
            
            return 0.0;
        }
        else { return payoff(path.back()); }
    }

    virtual bool isAsianOption() const { return false; } // returnes false in its non-overriden version
    virtual bool isAmericanOption() const { return false; } // same for American option

};


#endif
