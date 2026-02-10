#pragma once
#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"
#include <cmath>
#include <stdexcept>

class BlackScholesPricer {
private:
    // constructeur privé utilisé par ForVanilla() et ForDigital()
    BlackScholesPricer() = default;

    EuropeanVanillaOption* _opt = nullptr;
    EuropeanDigitalOption* _digitalOpt = nullptr;
    double _S = 0.0;
    double _r = 0.0;
    double _v = 0.0;


    double priceVanilla() const;
    double priceDigital() const;

    
    static BlackScholesPricer ForVanilla(EuropeanVanillaOption* option,
        double asset_price,
        double interest_rate,
        double volatility);

    static BlackScholesPricer ForDigital(EuropeanDigitalOption* option,
        double asset_price,
        double interest_rate,
        double volatility);

public:

    BlackScholesPricer(Option* option,
        double asset_price,
        double interest_rate,
        double volatility)
        : _S(asset_price), _r(interest_rate), _v(volatility)
    {
        if (!option)
            throw std::invalid_argument("Null option pointer");

        if (asset_price <= 0.0)
            throw std::invalid_argument("Asset price must be > 0");
        if (volatility <= 0.0)
            throw std::invalid_argument("Volatility must be > 0");

        // On utilise dynamic_cast pour vérifier dynamiquement le type réel de *option*.
        // Si 'option' pointe réellement vers un objet de type EuropeanVanillaOption
        // (ou d'une classe dérivée), alors dynamic_cast retourne un pointeur valide.
        // Sinon, il retourne nullptr.
        //
        // Cela permet de savoir à l'exécution si l'option fournie est :
        //   - une option vanille (Call/Put), dans ce cas v ≠ nullptr
        //   - ou non, dans ce cas v == nullptr.
        //
        // Grâce à cette vérification dynamique, on peut affecter correctement
        // _opt ou _digitalOpt, et ainsi choisir ensuite le bon calcul de prix.

        if (auto* v = dynamic_cast<EuropeanVanillaOption*>(option)) {
            _opt = v;
            _digitalOpt = nullptr;
            return;
        }
        // Vérifie si 'option' est en réalité une EuropeanDigitalOption.
        // dynamic_cast retourne un pointeur valide si c'est le cas,
        // sinon nullptr. Cela permet de détecter le type d'option.

        if (auto* d = dynamic_cast<EuropeanDigitalOption*>(option)) {
            _opt = nullptr;
            _digitalOpt = d;
            return;
        }

        throw std::invalid_argument("Unknown option type");
    }

    BlackScholesPricer(EuropeanVanillaOption* option,
        double asset_price,
        double interest_rate,
        double volatility)
        : _opt(option), _digitalOpt(nullptr),
        _S(asset_price), _r(interest_rate), _v(volatility)
    {
        if (!option) throw std::invalid_argument("Vanilla option pointer is null");
        if (asset_price <= 0.0) throw std::invalid_argument("Asset price must be > 0");
        if (volatility <= 0.0) throw std::invalid_argument("Volatility must be > 0");
    }
    BlackScholesPricer(EuropeanDigitalOption* option,
        double asset_price,
        double interest_rate,
        double volatility)
        : _opt(nullptr), _digitalOpt(option),
        _S(asset_price), _r(interest_rate), _v(volatility)
    {
        if (!option) throw std::invalid_argument("Digital option pointer is null");
        if (asset_price <= 0.0) throw std::invalid_argument("Asset price must be > 0");
        if (volatility <= 0.0) throw std::invalid_argument("Volatility must be > 0");
    }
    
    double operator()() const;
    
    double delta() const;

    static double norm_cdf(double x); // norm_cdf retourne la fonction de répartition  de la loi normale utilisée pour les formules Black–Scholes 
    static double norm_pdf(double x);   //norm_pdf(x) retourne la densité de probabilité de la loi normale qui est utilisée notamment pour les greeks


    
};
