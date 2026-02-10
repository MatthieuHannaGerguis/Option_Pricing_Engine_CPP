#include "BlackScholesPricer.h"
#include <iostream>
#include <numbers>
#include <cmath>
#include <stdexcept>

constexpr double PI = 3.14159265358979323846;

//cdf normal
// on a la fonction de répartition de la loi normale standard N(x), renvoie P(Z ≤ x) où Z ~ N(0,1). 
double BlackScholesPricer::norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

double BlackScholesPricer::norm_pdf(double x) { //la densité de probabilité de la loi normale greeks et digital
    return (1.0 / std::sqrt(2.0 * PI)) * std::exp(-0.5 * x * x);
}

// constructeur statique vanilla
BlackScholesPricer BlackScholesPricer::ForVanilla(
    EuropeanVanillaOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
{
    if (!option)
        throw std::invalid_argument("Vanilla option pointer is null");
    if (asset_price <= 0.0)
        throw std::invalid_argument("Asset price must be > 0");
    if (volatility <= 0.0)
        throw std::invalid_argument("Volatility must be > 0");

    BlackScholesPricer p;
    p._opt = option;
    p._digitalOpt = nullptr;
    p._S = asset_price;
    p._r = interest_rate;
    p._v = volatility;
    return p;
}


// constructeur statique digital
BlackScholesPricer BlackScholesPricer::ForDigital(
    EuropeanDigitalOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
{
    if (!option)
        throw std::invalid_argument("Digital option pointer is null");
    if (asset_price <= 0.0)
        throw std::invalid_argument("Asset price must be > 0");
    if (volatility <= 0.0)
        throw std::invalid_argument("Volatility must be > 0");

    BlackScholesPricer p;
    p._opt = nullptr;
    p._digitalOpt = option;
    p._S = asset_price;
    p._r = interest_rate;
    p._v = volatility;
    return p;
}

// prix vanilla
double BlackScholesPricer::priceVanilla() const {
    if (!_opt){
        throw std::runtime_error("No vanilla option assigned to pricer");
    }
    const double T = _opt->getExpiry();
    const double K = _opt->getStrike();
    const auto type = _opt->GetOptionType();

    if (T <= 0.0){ return _opt->payoff(_S);
    }

    const double sqrtT = std::sqrt(T);
    const double volSqrtT = _v * sqrtT;
    const double d1 = (std::log(_S / K) + (_r + 0.5 * _v * _v) * T) / volSqrtT;
    const double d2 = d1 - volSqrtT;

    if (type == EuropeanVanillaOption::optionType::call) {
        // C = S Φ(d1) - K e^{-rT} Φ(d2)
        return _S * norm_cdf(d1) - K * std::exp(-_r * T) * norm_cdf(d2);
    }
    else {
        // P = K e^{-rT} Φ(-d2) - S Φ(-d1)
        return K * std::exp(-_r * T) * norm_cdf(-d2) - _S * norm_cdf(-d1);
    }
        
}

// prix digital
double BlackScholesPricer::priceDigital() const {
    if (!_digitalOpt) {
        throw std::runtime_error("No digital option assigned to pricer");
    }
        

    const double T = _digitalOpt->getExpiry();
    const double K = _digitalOpt->getStrike();
    // auto évite d'avoir à écrire le type enum(type qui sert à définir un petit ensemble de valeurs possibles pour representer un choix) complet 
    const auto type = _digitalOpt->GetOptionType();

    if (T <= 0.0) return _digitalOpt->payoff(_S);

    const double sqrtT = std::sqrt(T);
    const double volSqrtT = _v * sqrtT;
    const double d1 = (std::log(_S / K) + (_r + 0.5 * _v * _v) * T) / volSqrtT;
    const double d2 = d1 - volSqrtT;
    
    double price;

    if (type == EuropeanDigitalOption::optionType::digital_call) {
        // Digital call: e^{-rT} Φ(d2)
        price =  std::exp(-_r * T) * norm_cdf(d2);
    }
        
    else {
        // Digital call: e^{-rT} Φ(d2)
        price =  std::exp(-_r * T) * norm_cdf(-d2);
    }
    price = std::round(price * 1'000'000.0) / 1'000'000.0; // on arrondi a 6 decimales apres la virgules pour digitale

    return price;
}


double BlackScholesPricer::operator()() const
{
    double result;

    if (_opt)
        result = priceVanilla();
    else if (_digitalOpt)
        result = priceDigital();
    else
        throw std::runtime_error("No option assigned to pricer");

    // on arrondi à 6 décimales
    result = std::round(result * 1'000'000.0) / 1'000'000.0;

    return result;
}



// delta vanilla
double BlackScholesPricer::delta() const {
    if (_opt) {
        const double T = _opt->getExpiry();
        const double K =
            
            _opt->getStrike();

        if (T <= 0.0) {
            return (_opt->GetOptionType() == EuropeanVanillaOption::optionType::call)
                ? ((_S > K) ? 1.0 : 0.0)
                : ((_S < K) ? -1.0 : 0.0);
        }

        const double d1 = (std::log(_S / K) + (_r + 0.5 * _v * _v) * T) / (_v * std::sqrt(T));
        return (_opt->GetOptionType() == EuropeanVanillaOption::optionType::call)
            ? 0.5 * std::erfc(-d1 / std::sqrt(2.0))     // N(d1)
            : 0.5 * std::erfc(-d1 / std::sqrt(2.0)) - 1.0; // N(d1) - 1
    }

    // DIGITALE witch means cash or noting
    if (_digitalOpt) {
        const double T = _digitalOpt->getExpiry();
        const double K = _digitalOpt->getStrike();

        if (T <= 0.0)
            return 0.0;

        const double sqrtT = std::sqrt(T);
        const double d2 =
            (std::log(_S / K) + (_r - 0.5 * _v * _v) * T) / (_v * sqrtT);

        double result = std::exp(-_r * T) * norm_pdf(d2) / (_S * _v * sqrtT);
        if (_digitalOpt->GetOptionType() == EuropeanDigitalOption::optionType::digital_put)
            result = -result;

        return result;
    }

    // Si rien n'est initialisé
    throw std::runtime_error("Delta not available");
}
