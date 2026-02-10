#pragma once
#ifndef BLACKSCHOLESMCPRICER_H
#define BLACKSCHOLESMCPRICER_H

#include "Option.h"
#include <vector>

class BlackScholesMCPricer {
private:
    Option* _option;                     
    double _sumDiscountedPayoffs;         // somme des payoffs actualisés
    int _nb_paths;
    double _interest_rate;

    double _sumDiscountedPayoffsSquare;   // somme des carres des payoffs pour la variance

    double _initial_price;                
    double _volatility;                   
                         

public:
   
    BlackScholesMCPricer(Option* option, double initial_price,
        double interest_rate, double volatility);  // constructeur

    int getNbPaths() const;     // retourne le nombre de trajectoires generees


    // génère nb_paths trajectoires supplémentaire et met a jour l'estimation
    void generate(int nb_paths);

    double operator()() const;

    // retourne l'intervalle de confiance à 95 %
    std::vector<double> confidenceInterval() const;
};

#endif


