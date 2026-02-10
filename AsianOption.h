#pragma once
#include "Option.h"
#include <vector>
#include <numeric>
#include <stdexcept>
#pragma warning(disable : 26495)

//Classe abstraite : AsianOption 
// Cette classe représente une option asiatique, dont le payoff dépend de la moyenne du sous-jacent sur plusieurs instants d’observation.
class AsianOption : public Option {
protected:
    double _strike;
    std::vector<double> _timeSteps;
    double getStrike() const;


public:
    // explicit empêche le constructeur d’être utilisé pour faire des conversions implicites de types.
    explicit AsianOption(double expiry,
        double strikeValue,
        const std::vector<double>& timestamps);

    // gettter
    const std::vector<double>& getTimeSteps() const;
    // on override pour indiquer que c'est une option asiatique
    bool isAsianOption() const override;

    double payoffPath(const std::vector<double>& path) const; // payoff of an Asian option along a given simulated price path

    virtual double payoff(double S) const = 0;     // methode virtuelle pure : les classes filles (Call / Put) doivent la définir

};