#pragma once
#include "Option.h"
#include "BinaryTree.h"
#include <cmath> // pour les operations mathematiques
#include <stdexcept>
#include <iostream>

class CRRPricer {
private:
    int _depth;
    double S0; // asset price
    double _up, _down, _interest_rate; // risk free rate
    double _q; // risk-neutral probability 
    Option* _option;
    bool _computed = false; // flag to avoid recomputation

    BinaryTree<double> _assetTree; // arbre pour le prix de l'actif
    BinaryTree<double> _valueTree; // arbre pour les valeurs de l'option
    BinaryTree<bool>   _exerciseTree;
protected:
    double getQ() const { return _q; }
    int getDepth() const { return _depth; }
    double getS0() const { return S0; }

public:
    // constructeur principal
    CRRPricer(Option* option, int depth, double asset_price,double up, double down, double interest_rate);

    // constructeur black-scholes
    CRRPricer(Option* option, int depth, double asset_price, double r, double volatility);


    // méthodes principales 
    void compute();                 // Procedure CRR
    
    double operator()(bool closed_form = false); // prix de l’option
    void display() const;      // affichage des arbres
    // getters 
    bool getExercise(int n, int i) const;
};
